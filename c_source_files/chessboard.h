#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <stdint.h>

void move_piece(const uint8_t origin[2], const uint8_t destination[2]);

uint8_t **get_possible_destinations(const uint8_t origin[2]);

#endif