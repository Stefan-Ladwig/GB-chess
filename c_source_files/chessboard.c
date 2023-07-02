#include <stdint.h>

enum chesspieces{no_Piece, King, Queen, Bishop, Knight, Rook, Pawn};

uint8_t chessboard[8][8] =
{
    {Rook,     Knight,   Bishop,   Queen,    King,     Bishop,   Knight,   Rook},
    {Pawn,     Pawn,     Pawn,     Pawn,     Pawn,     Pawn,     Pawn,     Pawn},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {Pawn,     Pawn,     Pawn,     Pawn,     Pawn,     Pawn,     Pawn,     Pawn},
    {Rook,     Knight,   Bishop,   Queen,    King,     Bishop,   Knight,   Rook}
};