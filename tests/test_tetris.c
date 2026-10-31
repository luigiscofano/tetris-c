#include "tetris.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static unsigned cases;
#define TEST(function) do { function(); ++cases; printf("PASS %s\n", #function); } while (0)

static unsigned occupied(const Tetris *game) {
    unsigned count = 0;
    for (int r = 0; r < TETRIS_ROWS; ++r)
        for (int c = 0; c < TETRIS_COLS; ++c) count += game->board[r][c] != 0;
    return count;
}

static void shapes_have_four_cells(void) {
    for (int kind = 0; kind < TETRIS_KINDS; ++kind)
        for (int rotation = -4; rotation < 8; ++rotation) {
            int count = 0;
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 4; ++c) count += tetris_cell((PieceKind)kind, rotation, r, c);
            assert(count == 4);
        }
    assert(!tetris_cell((PieceKind)99, 0, 0, 0));
    assert(!tetris_cell(PIECE_I, 0, -1, 0));
}

static void seed_is_reproducible(void) {
    Tetris a, b;
    tetris_init(&a, 42); tetris_init(&b, 42);
    for (int i = 0; i < 100; ++i) {
        assert(a.active.kind == b.active.kind && a.next == b.next);
        memset(a.board, 0, sizeof a.board); memset(b.board, 0, sizeof b.board);
        tetris_drop(&a); tetris_drop(&b);
    }
    tetris_init(&a, 0); assert(a.rng != 0);
}

static void bags_contain_seven_kinds(void) {
    for (uint32_t seed = 1; seed <= 100; ++seed) {
        Tetris game; tetris_init(&game, seed);
        for (int bag = 0; bag < 10; ++bag) {
            unsigned mask = 0;
            for (int piece = 0; piece < 7; ++piece) {
                mask |= 1U << game.active.kind;
                memset(game.board, 0, sizeof game.board);
                tetris_drop(&game);
            }
            assert(mask == 127U);
        }
    }
}

static void walls_and_floor_block_moves(void) {
    Tetris game; tetris_init(&game, 1);
    game.active = (Piece){PIECE_O, 0, -1, 18};
    assert(tetris_fits(&game, game.active));
    assert(!tetris_move(&game, -1));
    assert(tetris_move(&game, 1));
    game.active.x = 7;
    assert(!tetris_move(&game, 1));
    assert(!tetris_fits(&game, (Piece){PIECE_O, 0, 3, 19}));
    assert(!tetris_fits(&game, (Piece){PIECE_I, 0, INT_MAX, INT_MIN}));
    assert(!tetris_move(&game, INT_MAX));
    assert(!tetris_rotate(&game, INT_MIN));
}

static void board_collision_is_atomic(void) {
    Tetris game; tetris_init(&game, 1);
    game.active = (Piece){PIECE_O, 0, 3, 5};
    game.board[5][6] = 1;
    Piece before = game.active;
    assert(!tetris_move(&game, 1));
    assert(game.active.x == before.x && game.active.y == before.y);
    assert(occupied(&game) == 1);
}

static void rotation_cycles_and_kicks(void) {
    Tetris game; tetris_init(&game, 1);
    game.active = (Piece){PIECE_T, 0, 3, 5};
    for (int i = 0; i < 4; ++i) assert(tetris_rotate(&game, 1));
    assert(game.active.rotation == 0 && game.active.x == 3 && game.active.y == 5);
    assert(tetris_rotate(&game, -1)); assert(game.active.rotation == 3);
    game.active = (Piece){PIECE_I, 1, -2, 5};
    assert(tetris_fits(&game, game.active));
    assert(tetris_rotate(&game, -1));
    assert(game.active.x == 0 && game.active.rotation == 0);
    game.active = (Piece){PIECE_T, 0, 3, 18};
    Piece before = game.active;
    assert(!tetris_rotate(&game, 1));
    assert(game.active.rotation == before.rotation && game.active.x == before.x);
}

static void drops_and_locking(void) {
    Tetris game; tetris_init(&game, 1);
    game.active = (Piece){PIECE_O, 0, 3, 0};
    assert(tetris_step(&game, true)); assert(game.score == 1);
    assert(tetris_step(&game, false)); assert(game.score == 1);
    assert(tetris_drop(&game) == 16); assert(game.score == 33);
    assert(occupied(&game) == 4 && game.board[19][4] == PIECE_O + 1);
    game.active = (Piece){PIECE_O, 0, -1, 18};
    assert(!tetris_step(&game, false));
    assert(occupied(&game) == 8);
}

static void removes_one_to_four_lines(void) {
    static const unsigned points[] = {0, 100, 300, 500, 800};
    for (unsigned count = 1; count <= 4; ++count) {
        Tetris game; tetris_init(&game, 4);
        for (unsigned r = TETRIS_ROWS - count; r < TETRIS_ROWS; ++r)
            for (int c = 0; c < TETRIS_COLS; ++c) game.board[r][c] = c == 5 ? 0 : 1;
        game.active = (Piece){PIECE_I, 1, 3, 16};
        assert(tetris_drop(&game) == 0);
        assert(game.lines == count && game.score == points[count]);
        assert(occupied(&game) == 4U - count);
    }
}

static void compacts_nonadjacent_lines(void) {
    Tetris game; tetris_init(&game, 4);
    for (int c = 0; c < TETRIS_COLS; ++c) {
        game.board[17][c] = c == 5 ? 0 : 1;
        game.board[19][c] = c == 5 ? 0 : 1;
    }
    game.board[16][0] = 7;
    game.active = (Piece){PIECE_I, 1, 3, 16};
    tetris_drop(&game);
    assert(game.lines == 2 && game.score == 300);
    assert(game.board[18][0] == 7 && occupied(&game) == 3);
}

static void level_uses_preclear_multiplier(void) {
    Tetris game; tetris_init(&game, 4); game.lines = 9;
    for (int c = 0; c < TETRIS_COLS; ++c) game.board[19][c] = c == 5 ? 0 : 1;
    game.active = (Piece){PIECE_I, 1, 3, 16}; tetris_drop(&game);
    assert(game.lines == 10 && game.score == 100);
    assert(tetris_level(&game) == 2 && tetris_interval_ms(&game) == 750);
    game.lines = 140; assert(tetris_interval_ms(&game) == 100);
    game.lines = 100000; assert(tetris_interval_ms(&game) == 100);
}

static void pause_and_topout(void) {
    Tetris game; tetris_init(&game, 4); tetris_pause(&game);
    Tetris before = game;
    assert(!tetris_move(&game, -1)); assert(!tetris_rotate(&game, 1));
    assert(!tetris_step(&game, true)); assert(tetris_drop(&game) == 0);
    assert(memcmp(&game, &before, sizeof game) == 0);
    tetris_pause(&game); assert(!game.paused);
    for (int c = 3; c <= 6; ++c) { game.board[0][c] = 1; game.board[1][c] = 1; }
    game.active = (Piece){PIECE_O, 0, -1, 18};
    tetris_drop(&game); assert(game.game_over);
    before = game;
    tetris_pause(&game); tetris_move(&game, 1); tetris_drop(&game);
    assert(memcmp(&game, &before, sizeof game) == 0);
    tetris_init(&game, 4); assert(!game.game_over && occupied(&game) == 0);
}

static void stress_preserves_invariants(void) {
    for (uint32_t seed = 1; seed <= 100; ++seed) {
        Tetris game; tetris_init(&game, seed);
        uint32_t sequence = seed;
        for (int i = 0; i < 2000; ++i) {
            sequence = sequence * UINT32_C(1664525) + UINT32_C(1013904223);
            unsigned before = occupied(&game), lines = game.lines;
            switch ((sequence >> 16) % 8U) {
                case 0: tetris_move(&game, -1); break;
                case 1: tetris_move(&game, 1); break;
                case 2: tetris_rotate(&game, 1); break;
                case 3: tetris_rotate(&game, -1); break;
                case 4: tetris_step(&game, true); break;
                case 5: tetris_pause(&game); break;
                case 6: tetris_drop(&game); break;
                default: tetris_step(&game, false); break;
            }
            unsigned after = occupied(&game);
            assert(after + (game.lines - lines) * 10U == before ||
                   after + (game.lines - lines) * 10U == before + 4U);
            for (int r = 0; r < TETRIS_ROWS; ++r)
                for (int c = 0; c < TETRIS_COLS; ++c) assert(game.board[r][c] <= 7);
            if (game.game_over) tetris_init(&game, seed + (uint32_t)i);
            else assert(tetris_fits(&game, game.active));
        }
    }
}

int main(void) {
    TEST(shapes_have_four_cells); TEST(seed_is_reproducible);
    TEST(bags_contain_seven_kinds); TEST(walls_and_floor_block_moves);
    TEST(board_collision_is_atomic); TEST(rotation_cycles_and_kicks);
    TEST(drops_and_locking); TEST(removes_one_to_four_lines);
    TEST(compacts_nonadjacent_lines); TEST(level_uses_preclear_multiplier);
    TEST(pause_and_topout); TEST(stress_preserves_invariants);
    printf("PASS %u grupos; stress de 200000 acoes\n", cases);
    return 0;
}
