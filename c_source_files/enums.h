#ifndef ENUMS_H
#define ENUMS_H

enum colored_chesspiece{w_King = 1, w_Queen, w_Bishop, w_Knight, w_Rook, w_Pawn, 
                        b_King, b_Queen, b_Bishop, b_Knight, b_Rook, b_Pawn};

enum chesspiece{no_Piece, King, Queen, Bishop, Knight, Rook, Pawn};

enum color{white, black};

enum event{no_Event, Castles, Promotion, En_passaint, Checkmate, Remis};

#endif