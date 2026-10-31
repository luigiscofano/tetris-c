#define _POSIX_C_SOURCE 200809L
#include "terminal.h"
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <conio.h>
#include <io.h>
#include <windows.h>
static DWORD original_mode;
static HANDLE output;
#else
#include <sys/select.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
static struct termios original_mode;
#endif
static bool opened;
static volatile sig_atomic_t interrupted;
static void on_signal(int number) { (void)number; interrupted = 1; }

void terminal_close(void) {
    if (!opened) return;
    fputs("\033[0m\033[?25h\033[?1049l", stdout);
    fflush(stdout);
#ifdef _WIN32
    SetConsoleMode(output, original_mode);
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &original_mode);
#endif
    opened = false;
}

bool terminal_open(void) {
#ifdef _WIN32
    if (!_isatty(_fileno(stdin)) || !_isatty(_fileno(stdout))) return false;
    output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleMode(output, &original_mode)) return false;
    if (!SetConsoleMode(output, original_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) return false;
#else
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) return false;
    if (tcgetattr(STDIN_FILENO, &original_mode) != 0) return false;
    struct termios mode = original_mode;
    mode.c_lflag &= (tcflag_t)~(ICANON | ECHO);
    mode.c_cc[VMIN] = 0;
    mode.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &mode) != 0) return false;
    signal(SIGHUP, on_signal);
#endif
    opened = true;
    atexit(terminal_close);
    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);
    fputs("\033[?1049h\033[?25l\033[2J", stdout);
    return true;
}

bool terminal_interrupted(void) { return interrupted != 0; }

int terminal_key(void) {
#ifdef _WIN32
    if (!_kbhit()) return -1;
    int key = _getch();
    if (key == 0 || key == 224) {
        switch (_getch()) {
            case 75: return 'a'; case 77: return 'd';
            case 72: return 'w'; case 80: return 's'; default: return -1;
        }
    }
    return key;
#else
    static int escape_state;
    fd_set input;
    FD_ZERO(&input); FD_SET(STDIN_FILENO, &input);
    struct timeval wait = {0, 0};
    if (select(STDIN_FILENO + 1, &input, NULL, NULL, &wait) <= 0) return -1;
    unsigned char key;
    if (read(STDIN_FILENO, &key, 1) != 1) return -1;
    if (key == 27) { escape_state = 1; return -2; }
    if (escape_state == 1 && key == '[') { escape_state = 2; return -2; }
    if (escape_state == 2) {
        escape_state = 0;
        switch (key) {
            case 'A': return 'w'; case 'B': return 's';
            case 'C': return 'd'; case 'D': return 'a'; default: return -2;
        }
    }
    escape_state = 0;
    return key;
#endif
}

uint64_t terminal_now_ms(void) {
#ifdef _WIN32
    return (uint64_t)GetTickCount64();
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (uint64_t)now.tv_sec * 1000U + (uint64_t)now.tv_nsec / 1000000U;
#endif
}

void terminal_wait(void) {
#ifdef _WIN32
    Sleep(16);
#else
    struct timespec delay = {0, 16000000L};
    nanosleep(&delay, NULL);
#endif
}

void terminal_draw(const Tetris *game, bool animated) {
    char cells[TETRIS_ROWS][TETRIS_COLS];
    for (int row = 0; row < TETRIS_ROWS; ++row)
        for (int col = 0; col < TETRIS_COLS; ++col)
            cells[row][col] = game->board[row][col] ?
                tetris_piece_name((PieceKind)(game->board[row][col] - 1)) : '.';
    /* A peça ativa é uma sobreposição; não altera as células já fixadas. */
    if (!game->game_over) {
        for (int row = 0; row < 4; ++row)
            for (int col = 0; col < 4; ++col) {
                int y = game->active.y + row, x = game->active.x + col;
                if (tetris_cell(game->active.kind, game->active.rotation, row, col) &&
                    y >= 0 && y < TETRIS_ROWS && x >= 0 && x < TETRIS_COLS)
                    cells[y][x] = tetris_piece_name(game->active.kind);
            }
    }
    if (animated) fputs("\033[H", stdout);
    printf("TETRIS EM C | laboratorio de programacao\n");
    printf("Pontos: %-8" PRIu64 " Linhas: %-4u Nivel: %-3u\n", game->score, game->lines, tetris_level(game));
    printf("Proxima: %c | %-12s\n", tetris_piece_name(game->next),
        game->game_over ? "FIM DE JOGO" : game->paused ? "PAUSADO" : "JOGANDO");
    puts("+--------------------+");
    for (int row = 0; row < TETRIS_ROWS; ++row) {
        putchar('|');
        for (int col = 0; col < TETRIS_COLS; ++col) {
            char cell = cells[row][col];
            if (animated && cell != '.') {
                const char *palette = "IOTSZJL";
                int index = 0;
                while (palette[index] && palette[index] != cell) ++index;
                printf("\033[%dm%c%c\033[0m", 31 + index % 6, cell, cell);
            } else printf("%c%c", cell == '.' ? ' ' : cell, cell == '.' ? ' ' : cell);
        }
        puts("|");
    }
    puts("+--------------------+");
    puts("A/D ou setas: mover | W/Z: girar | S: descer");
    puts("Espaco: queda | P: pausa | R: reiniciar | Q: sair");
    if (animated) fputs("\033[J", stdout);
    fflush(stdout);
}
