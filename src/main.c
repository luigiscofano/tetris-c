#include "terminal.h"
#include "tetris.h"
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void help(void) {
    puts("Uso: tetris [--seed N] [--replay ARQUIVO] [--help] [--version]\n"
         "Terminal interativo: A/D ou setas movem; W/Z giram; S desce;\n"
         "Espaco derruba; P pausa; R reinicia com a mesma seed; Q sai.\n"
         "Replay: a d w z s x . p r q; ponto avanca a gravidade.\n"
         "Comentarios iniciam com #. Espacos e quebras de linha sao ignorados.");
}

static bool command(Tetris *game, int key, uint32_t seed) {
    switch (tolower((unsigned char)key)) {
        case 'a': tetris_move(game, -1); break;
        case 'd': tetris_move(game, 1); break;
        case 'w': tetris_rotate(game, 1); break;
        case 'z': tetris_rotate(game, -1); break;
        case 's': tetris_step(game, true); break;
        case 'x': case ' ': tetris_drop(game); break;
        case '.': tetris_step(game, false); break;
        case 'p': tetris_pause(game); break;
        case 'r': tetris_init(game, seed); break;
        case 'q': return false;
        default: break;
    }
    return true;
}

static int replay(Tetris *game, const char *path, uint32_t seed) {
    FILE *input = fopen(path, "r");
    if (!input) { perror(path); return 1; }
    int key;
    while ((key = fgetc(input)) != EOF) {
        if (key == '#') {
            do { key = fgetc(input); } while (key != '\n' && key != EOF);
            continue;
        }
        if (isspace((unsigned char)key)) continue;
        if (!strchr("adwzsx.prqADWZSXPRQ", key)) {
            fprintf(stderr, "Comando de replay invalido: 0x%02x\n", (unsigned)key);
            fclose(input); return 1;
        }
        if (!command(game, key, seed)) break;
    }
    if (ferror(input)) { fclose(input); fputs("Falha ao ler replay.\n", stderr); return 1; }
    fclose(input);
    terminal_draw(game, false);
    printf("seed=%" PRIu32 " score=%" PRIu64 " lines=%u level=%u game_over=%d\n",
           seed, game->score, game->lines, tetris_level(game), game->game_over);
    return 0;
}

int main(int argc, char **argv) {
    uint32_t seed = (uint32_t)time(NULL);
    const char *path = NULL;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) { help(); return 0; }
        if (strcmp(argv[i], "--version") == 0) { puts("tetris-c 1.0.0"); return 0; }
        if (strcmp(argv[i], "--seed") == 0 && i + 1 < argc) {
            const char *value = argv[++i];
            char *end;
            errno = 0;
            unsigned long long parsed = strtoull(value, &end, 10);
            if (*value == '\0' || !isdigit((unsigned char)*value) || *end != '\0' ||
                errno == ERANGE || parsed > UINT32_MAX) {
                fputs("Seed invalida: use inteiro de 0 a 4294967295.\n", stderr); return 2;
            }
            seed = (uint32_t)parsed;
        } else if (strcmp(argv[i], "--replay") == 0 && i + 1 < argc) path = argv[++i];
        else { fprintf(stderr, "Opcao invalida ou sem argumento: %s\n", argv[i]); return 2; }
    }
    Tetris game;
    tetris_init(&game, seed);
    if (path) return replay(&game, path, seed);
    if (!terminal_open()) {
        fputs("Abra em um terminal ANSI interativo ou use --replay ARQUIVO.\n", stderr); return 1;
    }
    bool playing = true;
    uint64_t last_tick = terminal_now_ms();
    while (playing && !terminal_interrupted()) {
        int key;
        while ((key = terminal_key()) != -1) {
            if (key == -2) continue;
            if (!command(&game, key, seed)) { playing = false; break; }
            if (key == 'p' || key == 'P' || key == 'r' || key == 'R' || key == ' ' || key == 'x' || key == 'X')
                last_tick = terminal_now_ms();
        }
        uint64_t now = terminal_now_ms();
        if (game.paused || game.game_over) last_tick = now;
        else if (now - last_tick >= tetris_interval_ms(&game)) {
            tetris_step(&game, false);
            last_tick = now;
        }
        terminal_draw(&game, true);
        terminal_wait();
    }
    terminal_close();
    return 0;
}
