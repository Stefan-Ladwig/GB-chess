#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <stdint.h>

extern enum event{no_Event, Promotion, En_passant, Checkmate, Stalemate};

uint8_t get_piece(uint8_t x, uint8_t y);

bool get_color(uint8_t colored_piece);

bool piece_on_square(uint8_t x, uint8_t y);

uint8_t move_piece_board(const uint8_t origin_x, const uint8_t origin_y,
                      const uint8_t destination_x, const uint8_t destination_y);

bool move_is_legal(uint8_t origin_x, uint8_t origin_y, uint8_t destination_x, uint8_t destination_y);

#endif