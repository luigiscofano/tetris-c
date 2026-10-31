#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdint.h>

#define TETRIS_ROWS 20
#define TETRIS_COLS 10
#define TETRIS_KINDS 7

typedef enum { PIECE_I, PIECE_O, PIECE_T, PIECE_S, PIECE_Z, PIECE_J, PIECE_L } PieceKind;
typedef struct { PieceKind kind; int rotation, x, y; } Piece;
typedef struct {
    uint8_t board[TETRIS_ROWS][TETRIS_COLS];
    Piece active;
    PieceKind next;
    PieceKind bag[TETRIS_KINDS];
    int bag_index;
    uint32_t rng;
    uint64_t score;
    unsigned lines;
    bool paused, game_over;
} Tetris;

/* O motor não lê teclado, não desenha e não consulta o relógio. */
void tetris_init(Tetris *game, uint32_t seed);
bool tetris_cell(PieceKind kind, int rotation, int row, int col);
bool tetris_fits(const Tetris *game, Piece piece);
bool tetris_move(Tetris *game, int dx);
bool tetris_rotate(Tetris *game, int direction);
bool tetris_step(Tetris *game, bool soft_drop);
int tetris_drop(Tetris *game);
void tetris_pause(Tetris *game);
unsigned tetris_level(const Tetris *game);
unsigned tetris_interval_ms(const Tetris *game);
char tetris_piece_name(PieceKind kind);

#endif
