#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <stdint.h>

uint8_t get_piece(uint8_t x, uint8_t y);

bool piece_on_square(uint8_t x, uint8_t y);

void move_piece(const uint8_t origin[2], const uint8_t destination[2]);

uint8_t **get_possible_destinations(const uint8_t origin[2]);

bool move_is_legal(uint8_t origin_x, uint8_t origin_y, uint8_t destination_x, uint8_t destination_y);

#endif