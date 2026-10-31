#ifndef TERMINAL_H
#define TERMINAL_H
#include <stdbool.h>
#include <stdint.h>
#include "tetris.h"
bool terminal_open(void);
void terminal_close(void);
int terminal_key(void);
uint64_t terminal_now_ms(void);
void terminal_wait(void);
bool terminal_interrupted(void);
void terminal_draw(const Tetris *game, bool animated);
#endif
