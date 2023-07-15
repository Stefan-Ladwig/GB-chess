#ifndef GAME_H
#define GAME_H
#include <stdbool.h>
#include <stdint.h>

extern bool player;

void await_input();

void title_screen();

void init_game();

void handle_input();

void handle_endgame(uint8_t event);

#endif