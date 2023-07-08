#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

enum colored_chesspiece{w_King = 1, w_Queen, w_Bishop, w_Knight, w_Rook, w_Pawn, 
                        b_King, b_Queen, b_Bishop, b_Knight, b_Rook, b_Pawn};

enum chesspiece{no_Piece, King, Queen, Bishop, Knight, Rook, Pawn};

enum color{white, black};

enum event{no_Event, Castles, Promotion, En_passaint, Checkmate, Stalemate};

uint8_t chessboard[8][8] =
{
    {b_Rook,   b_Knight, b_Bishop, b_Queen,  b_King,   b_Bishop, b_Knight, b_Rook},
    {b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn,   b_Pawn},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece, no_Piece},
    {w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn,   w_Pawn},
    {w_Rook,   w_Knight, w_Bishop, w_Queen,  w_King,   w_Bishop, w_Knight, w_Rook}
};

uint8_t king_positions[2][2] = { {7, 4}, {0, 4} };

bool castle_pieces_moved[2][3] = { {false, false, false}, {false, false, false} };

uint8_t en_passaint_square[2] = { UINT8_MAX, UINT8_MAX };


uint8_t get_piece(uint8_t x, uint8_t y)
{
    return chessboard[x][y];
}


bool get_color(uint8_t colored_piece)
{
    return (colored_piece > 6);
}


uint8_t get_colorless_piece(uint8_t x, uint8_t y)
{
    return get_piece(x, y) - 6 * get_color(get_piece(x, y));
}


void set_piece(uint8_t x, uint8_t y, uint8_t piece)
{
    chessboard[x][y] = piece;
}


bool piece_on_square(uint8_t x, uint8_t y)
{
    return chessboard[x][y] != no_Piece;
}

bool index_out_of_range(uint8_t index)
{
    return (index > 7); 
}


bool pawn_moved(uint8_t row, bool color)
{
    return !((color == white && row == 6) || (color == black && row == 1));
}


bool pawn_jumped(const uint8_t origin_x,
                 const uint8_t destination_x, const uint8_t destination_y)
{
    return (get_colorless_piece(destination_x, destination_y) == Pawn && abs(destination_x - origin_x) == 2);
}


bool castling(const uint8_t origin_x, const uint8_t origin_y, const uint8_t destination_y)
{
    uint8_t piece = get_colorless_piece(origin_x, origin_y);
    return (piece == King && abs(destination_y - origin_y) == 2);
}


bool pawn_promotes(uint8_t row, uint8_t col)
{
    uint8_t piece = get_piece(row, col);
    piece -= 6 * get_color(piece);
    if ( (piece == Pawn) && (row == 0 || row == 7) )
        return true;
    return false;
}


bool en_passaint(const uint8_t origin_x, const uint8_t origin_y,
                 const uint8_t destination_x, const uint8_t destination_y)
{
    uint8_t friendly_piece = get_colorless_piece(origin_x, origin_y);
    uint8_t enemy_piece = get_colorless_piece(destination_x, destination_y);

    return (friendly_piece == Pawn && enemy_piece == no_Piece && destination_y - origin_y != 0);
}


void update_king_position(uint8_t destination_x, uint8_t destination_y)
{
    if (get_piece(destination_x, destination_y) == w_King)
    {
        king_positions[white][0] = destination_x;
        king_positions[white][1] = destination_y;
    }
    else if (get_piece(destination_x, destination_y) == b_King)
    {
        king_positions[black][0] = destination_x;
        king_positions[black][1] = destination_y;
    }
}


void update_castling_rook(uint8_t x, uint8_t y, bool color)
{
    if (x != 7 * !color)
        return;

    if (y == 0)
        castle_pieces_moved[color][0] = true;
    else if (y == 7)
        castle_pieces_moved[color][2] = true;
}


void update_castling_pieces(uint8_t x, uint8_t y)
{
    uint8_t piece = get_colorless_piece(x, y);
    bool color = get_color(get_piece(x, y));

    if (piece == King)
        castle_pieces_moved[color][1] = true;
    else if (piece == Rook)
        update_castling_rook(x, y, color);
}


bool stale_or_checkmate(bool color);
bool king_under_attack(bool color);


uint8_t move_piece_board(const uint8_t origin_x, const uint8_t origin_y,
                         const uint8_t destination_x, const uint8_t destination_y)
{
    bool color = get_color(get_piece(origin_x, origin_y));
    bool move_is_en_passant = en_passaint(origin_x, origin_y, destination_x, destination_y);
    bool move_is_castling = castling(origin_x, origin_y, destination_y);
    
    set_piece(destination_x, destination_y, get_piece(origin_x, origin_y));
    set_piece(origin_x, origin_y, no_Piece);

    update_king_position(destination_x, destination_y);
    update_castling_pieces(destination_x, destination_y);

    if (pawn_jumped(origin_x, destination_x, destination_y))
    {
        en_passaint_square[0] = destination_x + 1 - 2 * color;
        en_passaint_square[1] = destination_y;
    }
    else
    {
        en_passaint_square[0] = UINT8_MAX;
        en_passaint_square[1] = UINT8_MAX;
    }

    if (move_is_castling)
    {
        bool castling_short = (destination_y == 6);
        set_piece(origin_x, 7 * castling_short, no_Piece);
        set_piece(origin_x, 3 + 2 * castling_short, Rook + 6 * color);
        return Castles;
    }

    if (pawn_promotes(destination_x, destination_y)) return Promotion;

    if (move_is_en_passant) return En_passaint;

    if (stale_or_checkmate(!color))
    {
        if (king_under_attack(!color))
            return Checkmate;
        else
            return Stalemate;
    }

    return no_Event;
}


void revert_move(const uint8_t origin_x, const uint8_t origin_y,
                 const uint8_t destination_x, const uint8_t destination_y,
                 const uint8_t piece_taken, const uint8_t event, const uint8_t en_passaint_buffer[2])
{
    set_piece(origin_x, origin_y, get_piece(destination_x, destination_y));
    set_piece(destination_x, destination_y, piece_taken);
    en_passaint_square[0] = en_passaint_buffer[0];
    en_passaint_square[1] = en_passaint_buffer[1];

    bool color = get_color(get_piece(origin_x, origin_y));

    switch (event)
    {
    case Castles:;
        bool castling_short = (destination_y == 6);
        set_piece(origin_x, 7 * castling_short, Rook + 6 * color);
        set_piece(origin_x, 3 + 2 * castling_short, no_Piece);
        break;
    case Promotion:;
        set_piece(origin_x, origin_y, Pawn + 6 * color);
        break;
    case En_passaint:;
        set_piece(origin_x ,destination_y, Pawn + 6 * color);
        break;
    }
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


void free_possible_destinations(uint8_t **possible_destinations)
{
    while (*possible_destinations != NULL)
    {
        free(*possible_destinations);
        possible_destinations++;
    }
}


bool is_possible_destination(uint8_t row, uint8_t col, uint8_t new_row, uint8_t new_col)
{
    return (!piece_on_square(new_row, new_col) ||
            get_color(get_piece(new_row, new_col)) != get_color(get_piece(row, col)));
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
            
            if ((!repeatable) || (repeatable && get_piece(new_row, new_col) != no_Piece))
                break;
        }
    }
}


void get_destinations_for_piece(uint8_t piece, uint8_t row, uint8_t col,
                                uint8_t ***possible_destinations, uint8_t *num_solutions)
{
    uint8_t new_row = row;
    uint8_t new_col = col;
    const bool color = get_color(piece);
    piece = piece - 6 * color;

    switch (piece)
    {
    case Pawn:
        for(uint8_t i = 0; i < 2; i++)
        {
            new_row = row + (2 * color - 1) * (i + 1);
            if (index_out_of_range(new_row))
                continue;

            if (get_piece(new_row, new_col) == no_Piece)
                add_possible_destination(possible_destinations, new_row, new_col, num_solutions);

            if (pawn_moved(row, color))
                break;
        }

        new_row = row - 1 + 2 * color;
        if (index_out_of_range(new_row))
            break;

        for(uint8_t i = 0; i < 2; i++)
        {
            new_col = col + 1 - 2 * i;
            if (index_out_of_range(new_col))
                continue;

            if ((piece_on_square(new_row, new_col) && get_color(get_piece(new_row, new_col)) != color)
                || (new_row == en_passaint_square[0] && new_col == en_passaint_square[1]))
                add_possible_destination(possible_destinations, new_row, new_col, num_solutions);
        }
        break;

    case Knight:;
        const int8_t knight_moves[8][2] = { {1, 2}, {2, 1}, {-1, 2}, {-2, 1},
                                            {1, -2}, {2, -1}, {-1, -2}, {-2, -1}};
        get_destinations_from_move_set(row, col, num_solutions, 8, knight_moves, false, possible_destinations);
        break;

    case Bishop:;
        const int8_t bishop_moves[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
        get_destinations_from_move_set(row, col, num_solutions, 4, bishop_moves, true, possible_destinations);
        break;

    case Rook:;
        const int8_t rook_moves[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
        get_destinations_from_move_set(row, col, num_solutions, 4, rook_moves, true, possible_destinations);
        break;

    case Queen:;
        const int8_t queen_moves[8][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1},
                                           {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
        get_destinations_from_move_set(row, col, num_solutions, 8, queen_moves, true, possible_destinations);
        break;
        
    case King:;
        const int8_t king_moves[8][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1},
                                           {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
        get_destinations_from_move_set(row, col, num_solutions, 8, king_moves, false, possible_destinations);
        
        if (castle_pieces_moved[color][1]) break;

        uint8_t row_king = 7 * !color;

        for (uint8_t i = 0; i < 3; i+=2)
        {
            if (castle_pieces_moved[color][i]) continue;

            bool squares_not_empty = false;
            for (uint8_t j = 1 + 2 * i; j < 3 + 2 * i + 1 - i / 2; j++)
            {
                if (piece_on_square(row_king, j))
                {
                    squares_not_empty = true;
                    break;
                }
            }

            if (squares_not_empty) continue;

            if (king_under_attack(color)) continue;
            
            bool king_attacked = false;
            for (int8_t k = 0; k < 2; k++)
            {
                uint8_t new_col_king = 4 + (k + 1) * (i - 1);
                set_piece(row_king, new_col_king, King + 6 * color);
                set_piece(row_king, new_col_king - i + 1, no_Piece);
                king_positions[color][1] = new_col_king;

                if (king_under_attack(color))
                {
                    set_piece(row_king, new_col_king, no_Piece);
                    set_piece(row_king, new_col_king - i + 1, King + 6 * color);
                    king_positions[color][1] = new_col_king - i + 1;
                    king_attacked = true;
                    break;
                }
            }
            if (king_attacked) continue;

            set_piece(row_king, 4 + 2 * (i - 1), no_Piece);
            set_piece(row_king, 4, King + 6 * color);
            king_positions[color][1] = 4;

            add_possible_destination(possible_destinations, row_king, 2 + 2 * i, num_solutions);
        }

        break;
    }
}


bool move_is_possible(uint8_t origin_x, uint8_t origin_y, uint8_t destination_x, uint8_t destination_y)
{
    uint8_t piece = get_piece(origin_x, origin_y);
    uint8_t **possible_destinations = malloc(sizeof(uint8_t*));
    *possible_destinations = (uint8_t*) NULL;
    uint8_t num_solutions = 0;
    get_destinations_for_piece(piece, origin_x, origin_y, &possible_destinations, &num_solutions);

    uint8_t i = 0;
    while (possible_destinations[i] != NULL)
    {
        if (possible_destinations[i][0] == destination_x &&
            possible_destinations[i][1] == destination_y)
        {
            free_possible_destinations(possible_destinations);
            free(possible_destinations);
            return true;
        }
        i++;
    }
    free_possible_destinations(possible_destinations);
    free(possible_destinations);
    return false;
}


bool king_under_attack(bool color)
{
    uint8_t x = king_positions[color][0];
    uint8_t y = king_positions[color][1];

    for (uint8_t piece = Queen; piece <= Pawn; piece++)
    {
        uint8_t colored_piece = piece + 6 * color;
        set_piece(x, y, colored_piece);
        uint8_t **possible_destinations = malloc(sizeof(uint8_t*));
        *possible_destinations = (uint8_t*) NULL;
        uint8_t num_solutions = 0; 
        get_destinations_for_piece(piece, x, y, &possible_destinations, &num_solutions);

        uint8_t i = 0;
        while (possible_destinations[i] != NULL)
        {
            if (get_piece(possible_destinations[i][0], possible_destinations[i][1]) == piece + 6 * (!color))
            {
                free_possible_destinations(possible_destinations);
                free(possible_destinations);
                set_piece(x, y, King + 6 * color);
                return true;
            }
            i++;            
        }
        free_possible_destinations(possible_destinations);
        free(possible_destinations);
    }
    set_piece(x, y, King + 6 * color);
    return false;
}


bool move_is_legal(uint8_t origin_x, uint8_t origin_y, uint8_t destination_x, uint8_t destination_y, bool move_possible)
{
    if (!move_possible)
        move_possible = move_is_possible(origin_x, origin_y, destination_x, destination_y);
    bool friendly_king_attacked = false;
    if (move_possible)
    {
        uint8_t piece_on_destination = get_piece(destination_x, destination_y);
        uint8_t en_passaint_buffer[2] = { en_passaint_square[0], en_passaint_square[1] };
        uint8_t event = move_piece_board(origin_x, origin_y, destination_x, destination_y);

        friendly_king_attacked = king_under_attack(get_color(get_piece(destination_x, destination_y)));

        revert_move(origin_x, origin_y, destination_x, destination_y, piece_on_destination, event, en_passaint_buffer);
    }
    return move_possible && !friendly_king_attacked;
}


bool stale_or_checkmate(bool color)
{
    for(uint8_t row = 0; row < 8; row++)
    {
        for(uint8_t col = 0; col < 8; col++)
        {
            uint8_t piece = get_piece(row, col);

            if (piece == no_Piece || get_color(piece) != color) continue;

            uint8_t **possible_destinations = malloc(sizeof(uint8_t*));
            *possible_destinations = (uint8_t*) NULL;
            uint8_t num_solutions = 0; 
            get_destinations_for_piece(piece, row, col, &possible_destinations, &num_solutions);
            
            uint8_t i = 0;
            while (possible_destinations[i] != NULL)
            {
                if (move_is_legal(row, col, possible_destinations[i][0], possible_destinations[i][1], true))
                {
                    free_possible_destinations(possible_destinations);
                    free(possible_destinations);
                    return false;
                }
                i++;            
            }
            free_possible_destinations(possible_destinations);
            free(possible_destinations);
        }
    }
    return true;
}


// void main()
// {
//     move_is_legal(1, 4, 2, 4);
//     move_piece_board(1, 4, 2, 4);
// }