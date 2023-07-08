#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <stdint.h>

extern enum chesspiece{no_Piece, King, Queen, Bishop, Knight, Rook, Pawn};

extern enum colored_chesspiece{w_King = 1, w_Queen, w_Bishop, w_Knight, w_Rook, w_Pawn, 
                               b_King, b_Queen, b_Bishop, b_Knight, b_Rook, b_Pawn};

extern enum event{no_Event, Castles, Promotion, En_passaint, Checkmate, Stalemate};

uint8_t get_piece(uint8_t x, uint8_t y);

void set_piece(uint8_t x, uint8_t y, uint8_t piece);

bool get_color(uint8_t colored_piece);

bool piece_on_square(uint8_t x, uint8_t y);

uint8_t move_piece_board(const uint8_t origin_x, const uint8_t origin_y,
                      const uint8_t destination_x, const uint8_t destination_y);

bool move_is_legal(uint8_t origin_x, uint8_t origin_y, uint8_t destination_x, uint8_t destination_y, bool move_possible);

#endif