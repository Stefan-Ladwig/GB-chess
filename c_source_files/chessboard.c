#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

enum colored_chesspiece{w_King, w_Queen, w_Bishop, w_Knight, w_Rook, w_Pawn, 
                        b_King, b_Queen, b_Bishop, b_Knight, b_Rook, b_Pawn};

enum chesspiece{no_Piece, King, Queen, Bishop, Knight, Rook, Pawn};

enum color{white, black};

uint8_t chessboard[8][8] =
{
    {w_Rook,   w_Knight, w_Bishop, w_Queen,  w_King,   w_Bishop, w_Knight, w_Rook},
    {w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn},
    {b_Rook,   b_Knight, b_Bishop, b_Queen,  b_King,   b_Bishop, b_Knight, b_Rook}
};


bool get_color(uint8_t colored_piece)
{
    return (colored_piece > 5);
}


bool index_out_of_range(uint8_t index)
{
    return (index > 7); 
}


bool pawn_didnt_move(uint8_t row, bool color)
{
    return ((color == white && row == 1) || (color == black && row == 6));
}


void move_piece(const uint8_t origin[2], const uint8_t destination[2])
{
    chessboard[destination[0]][destination[1]] = chessboard[origin[0]][origin[1]];
    chessboard[origin[0]][origin[1]] = no_Piece;
}


bool is_possible_destination(uint8_t row, uint8_t col, uint8_t new_row, uint8_t new_col)
{
    return (chessboard[new_row][new_col] == no_Piece ||
            get_color(chessboard[new_row][new_col]) != get_color(chessboard[row][col]));
}


void add_possible_destination(uint8_t ***possible_destinations, uint8_t new_row,
                              uint8_t new_col, uint8_t *num_solutions)
{
    *num_solutions = (*num_solutions) + 1;
    *possible_destinations = (uint8_t**) realloc(*possible_destinations, 
                                                ((*num_solutions) + 1) * sizeof(uint8_t*));
    uint8_t *new_destination = malloc(2*sizeof(uint8_t));
    new_destination[0] = new_row;
    new_destination[1] = new_col;
    (*possible_destinations)[(*num_solutions) - 1] = new_destination;
    (*possible_destinations)[(*num_solutions)] = (uint8_t*) NULL;
}


void get_destinations_from_move_set(uint8_t row, uint8_t col, uint8_t *num_solutions, uint8_t num_moves,
                                    uint8_t move_set[][2], bool repeatable, uint8_t ***possible_destinations)
{
    uint8_t new_row = 0;
    uint8_t new_col = 0;

    for (uint8_t i = 0; i < num_moves; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            new_row = row + (j + 1) * move_set[i][0];
            new_col = col + (j + 1) * move_set[i][1];
            if (index_out_of_range(new_row) || index_out_of_range(new_col))
                continue;
            
            if (is_possible_destination(row, col, new_row, new_col))
                add_possible_destination(possible_destinations, new_row, new_col, num_solutions);
            
            if ((!repeatable) || (repeatable && chessboard[new_row][new_col] != no_Piece))
                break;
        }
    }
}


uint8_t **get_possible_destinations(const uint8_t origin[2])
{
    uint8_t **possible_destinations = malloc(sizeof(uint8_t*));
    *possible_destinations = (uint8_t*) NULL;
    uint8_t num_solutions = 0;
    uint8_t row  = origin[0];
    uint8_t col = origin[1];
    uint8_t new_row = row;
    uint8_t new_col = col;
    uint8_t moving_piece = chessboard[row][col];
    const bool color = get_color(moving_piece);
    moving_piece = moving_piece + 1 - 6 * color;

    switch (moving_piece)
    {
    case Pawn:

        for(uint8_t i = 0; i < 2; i++)
        {
            new_row = row + 1 - 2 * color;
            if (index_out_of_range(new_row))
                break;

            if (chessboard[new_row][new_col] == no_Piece)
            {
                add_possible_destination(&possible_destinations, new_row, new_col, &num_solutions);
            }

            if (pawn_didnt_move(row, color))
                row = new_row;
            else
                break;
        }

        new_row = row + 1 - 2 * color;
        if (index_out_of_range(new_row))
            break;

        for(uint8_t i = 0; i < 2; i++)
        {
            new_col = col + 1 - 2 * i;
            if (index_out_of_range(new_col))
                break;

            if (chessboard[new_row][new_col] != no_Piece &&
                get_color(chessboard[new_row][new_col]) != color)
            {
                add_possible_destination(&possible_destinations, new_row, new_col, &num_solutions);
            }
        }
        break;

    case Knight:
        ;
        const int8_t knight_moves[8][2] = { {1, 2}, {2, 1}, {-1, 2}, {-2, 1},
                                            {1, -2}, {2, -1}, {-1, -2}, {-2, -1}};

        get_destinations_from_move_set(row, col, &num_solutions, 8, knight_moves, false, &possible_destinations);

        break;

    case Bishop:
        ;
        const int8_t bishop_moves[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

        get_destinations_from_move_set(row, col, &num_solutions, 4, bishop_moves, true, &possible_destinations);
    
        break;

    case Rook:
        ;
        const int8_t rook_moves[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

        get_destinations_from_move_set(row, col, &num_solutions, 4, bishop_moves, true, &possible_destinations);
    
        break;

    case Queen:
        ;
        const int8_t queen_moves[8][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1},
                                           {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

        get_destinations_from_move_set(row, col, &num_solutions, 8, bishop_moves, true, &possible_destinations);
    
        break;
        
    case King:
        ;
        const int8_t king_moves[8][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1},
                                           {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

        get_destinations_from_move_set(row, col, &num_solutions, 8, bishop_moves, false, &possible_destinations);
    
        break;
    }
    return possible_destinations;
}