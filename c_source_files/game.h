#ifndef GAME_H
#define GAME_H
#include <stdbool.h>

extern bool player;

void init_game();

void handle_input();

void handle_endgame(uint8_t event);

#endif