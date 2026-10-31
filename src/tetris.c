#include "tetris.h"
#include <string.h>

/* Máscaras próprias: cada peça ocupa quatro células em uma matriz 4x4. */
static const char *const shapes[TETRIS_KINDS] = {
    "...." "####" "...." "....",
    ".##." ".##." "...." "....",
    ".#.." "###." "...." "....",
    ".##." "##.." "...." "....",
    "##.." ".##." "...." "....",
    "#..." "###." "...." "....",
    "..#." "###." "...." "...."
};

char tetris_piece_name(PieceKind kind) {
    return kind >= PIECE_I && kind <= PIECE_L ? "IOTSZJL"[kind] : '?';
}

bool tetris_cell(PieceKind kind, int rotation, int row, int col) {
    if (kind < PIECE_I || kind > PIECE_L || row < 0 || row > 3 || col < 0 || col > 3)
        return false;
    rotation = ((rotation % 4) + 4) % 4;
    /* O quadrado permanece no mesmo lugar; as outras peças giram no 4x4. */
    if (kind != PIECE_O) {
        for (int turn = 0; turn < rotation; ++turn) {
            int previous_row = row;
            row = 3 - col;
            col = previous_row;
        }
    }
    return shapes[kind][row * 4 + col] == '#';
}

static uint32_t random_next(Tetris *game) {
    uint32_t value = game->rng;
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;
    game->rng = value;
    return value;
}

static PieceKind take_piece(Tetris *game) {
    if (game->bag_index == TETRIS_KINDS) {
        for (int i = 0; i < TETRIS_KINDS; ++i) game->bag[i] = (PieceKind)i;
        for (int i = TETRIS_KINDS - 1; i > 0; --i) {
            int j = (int)(random_next(game) % (uint32_t)(i + 1));
            PieceKind temporary = game->bag[i];
            game->bag[i] = game->bag[j];
            game->bag[j] = temporary;
        }
        game->bag_index = 0;
    }
    return game->bag[game->bag_index++];
}

bool tetris_fits(const Tetris *game, Piece piece) {
    if (piece.kind < PIECE_I || piece.kind > PIECE_L) return false;
    /* Limites prévios também evitam overflow para coordenadas inválidas. */
    if (piece.x < -3 || piece.x >= TETRIS_COLS || piece.y < -3 || piece.y >= TETRIS_ROWS)
        return false;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (!tetris_cell(piece.kind, piece.rotation, row, col)) continue;
            int x = piece.x + col, y = piece.y + row;
            if (x < 0 || x >= TETRIS_COLS || y < 0 || y >= TETRIS_ROWS) return false;
            if (game->board[y][x] != 0) return false;
        }
    }
    return true;
}

static void spawn(Tetris *game) {
    game->active = (Piece){game->next, 0, 3, 0};
    game->next = take_piece(game);
    game->game_over = !tetris_fits(game, game->active);
}

void tetris_init(Tetris *game, uint32_t seed) {
    memset(game, 0, sizeof(*game));
    game->rng = seed == 0 ? UINT32_C(0x6d2b79f5) : seed;
    game->bag_index = TETRIS_KINDS;
    game->next = take_piece(game);
    spawn(game);
}

unsigned tetris_level(const Tetris *game) { return game->lines / 10U + 1U; }
unsigned tetris_interval_ms(const Tetris *game) {
    unsigned level = tetris_level(game);
    return level >= 15U ? 100U : 800U - (level - 1U) * 50U;
}

static bool running(const Tetris *game) { return !game->paused && !game->game_over; }

bool tetris_move(Tetris *game, int dx) {
    if (!running(game) || (dx != -1 && dx != 1)) return false;
    Piece candidate = game->active;
    candidate.x += dx;
    if (!tetris_fits(game, candidate)) return false;
    game->active = candidate;
    return true;
}

bool tetris_rotate(Tetris *game, int direction) {
    if (!running(game) || (direction != -1 && direction != 1)) return false;
    static const int kicks[] = {0, -1, 1, -2, 2};
    Piece candidate = game->active;
    candidate.rotation = (candidate.rotation + direction + 4) % 4;
    for (unsigned i = 0; i < sizeof(kicks) / sizeof(kicks[0]); ++i) {
        candidate.x = game->active.x + kicks[i];
        if (tetris_fits(game, candidate)) {
            game->active = candidate;
            return true;
        }
    }
    return false;
}

static unsigned clear_lines(Tetris *game) {
    int destination = TETRIS_ROWS - 1;
    unsigned removed = 0;
    /* Compactação de baixo para cima: linhas consecutivas não são puladas. */
    for (int source = TETRIS_ROWS - 1; source >= 0; --source) {
        bool full = true;
        for (int col = 0; col < TETRIS_COLS; ++col)
            if (game->board[source][col] == 0) full = false;
        if (full) { ++removed; continue; }
        if (destination != source)
            memcpy(game->board[destination], game->board[source], TETRIS_COLS);
        --destination;
    }
    while (destination >= 0) memset(game->board[destination--], 0, TETRIS_COLS);
    return removed;
}

static void lock_piece(Tetris *game) {
    if (!tetris_fits(game, game->active)) { game->game_over = true; return; }
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            if (tetris_cell(game->active.kind, game->active.rotation, row, col))
                game->board[game->active.y + row][game->active.x + col] =
                    (uint8_t)(game->active.kind + 1);
    unsigned level = tetris_level(game);
    unsigned removed = clear_lines(game);
    static const unsigned points[] = {0, 100, 300, 500, 800};
    /* Um tabuleiro válido só pode completar até quatro linhas por peça. */
    if (removed <= 4) game->score += (uint64_t)points[removed] * level;
    game->lines += removed;
    spawn(game);
}

bool tetris_step(Tetris *game, bool soft_drop) {
    if (!running(game)) return false;
    Piece candidate = game->active;
    ++candidate.y;
    if (!tetris_fits(game, candidate)) { lock_piece(game); return false; }
    game->active = candidate;
    if (soft_drop) ++game->score;
    return true;
}

int tetris_drop(Tetris *game) {
    if (!running(game)) return 0;
    int distance = 0;
    Piece candidate = game->active;
    ++candidate.y;
    while (tetris_fits(game, candidate)) {
        game->active = candidate;
        ++candidate.y;
        ++distance;
    }
    game->score += (uint64_t)distance * 2U;
    lock_piece(game);
    return distance;
}

void tetris_pause(Tetris *game) {
    if (!game->game_over) game->paused = !game->paused;
}
