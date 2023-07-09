#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "chessboard.h"
#include "graphics.h"

const uint8_t DPAD_PRESSED = J_LEFT | J_RIGHT | J_UP | J_DOWN;

uint8_t joypad_state;
bool square_selected;
bool player;
bool replay_mode = false;
uint8_t event;
uint8_t **list_of_moves;
uint16_t num_moves;

struct pos {
    uint8_t x;
    uint8_t y;
} cursor, selection;


void init_cursor()
{
    selection.x = 0;
    selection.y = 0;
    
    cursor.x = 3;
    cursor.y = 6;

    move_cursor_sprites(cursor.x, cursor.y);
}


void init_list_of_moves()
{
    for (uint8_t i = 0; i < num_moves; i++)
    {
        free(list_of_moves[i]);
    }    

    num_moves = 0;
    list_of_moves = realloc(list_of_moves, 0);
}


void init_game()
{
    init_board();
    init_graphics();
    init_cursor();
    if (!replay_mode) init_list_of_moves();

    joypad_state = 0;
    square_selected = false;
    player = false;
    event = no_Event;
}


void save_move()
{
    num_moves++;
    list_of_moves = realloc(list_of_moves, num_moves * sizeof(uint8_t*));

    uint8_t *move = malloc(4 * sizeof(uint8_t));

    move[0] = cursor.x;
    move[1] = cursor.y;
    move[2] = selection.x;
    move[3] = selection.y;

    list_of_moves[num_moves - 1] = move;
}


void get_direction(uint8_t joypad_state, int8_t direction[2])
{
    switch (joypad_state)
    {
    case J_LEFT:
        direction[0] = -1;
        break;

    case J_RIGHT:
        direction[0] = 1;
        break;

    case J_UP:
        direction[1] = -1;
        break;

    case J_DOWN:
       direction[1] = 1;
        break;
    }
}


void move_cursor(uint8_t joypad_state)
{    
    int8_t dir[2] = {0};
    get_direction(joypad_state, dir);
    cursor.x = (cursor.x + dir[0] + 8) % 8;
    cursor.y = (cursor.y + dir[1] + 8) % 8;
}


void move_selection(uint8_t joypad_state)
{    
    int8_t dir[2] = {0};
    get_direction(joypad_state, dir);

    selection.x = (selection.x + dir[0] + 8) % 8;
    selection.y = (selection.y + dir[1] + 8) % 8;
}


void handle_dpad(uint8_t joypad_state)
{
    if (square_selected)
    {
        move_selection(joypad_state);
        move_selection_sprites(selection.x, selection.y);
    }
    else
    {
        move_cursor(joypad_state);
        move_cursor_sprites(cursor.x, cursor.y);
    }
}


void handle_promotion()
{
    uint8_t piece = Queen;
    bool color = player;
    draw_piece(selection.x, selection.y, Queen + 6 * color);
    set_piece(selection.y, selection.x, Queen + 6 * color);

    waitpadup();
    joypad_state = joypad();

    while (!(joypad_state & J_A))
    {
        if (joypad_state & J_LEFT || joypad_state & J_RIGHT)
        {
            if (joypad_state & J_RIGHT) piece++;
            else if (joypad_state & J_LEFT) piece--;
            piece = (piece - 2 + 4) % 4 + 2;

            draw_piece(selection.x, selection.y, piece + 6 * color);
            waitpadup();
        }
        joypad_state = joypad();
    }
    set_piece(selection.y, selection.x, piece + 6 * color);
}


void handle_button_a()
{
    if (!square_selected && piece_on_square(cursor.y, cursor.x) &&
        player == get_color(get_piece(cursor.y, cursor.x)))
    {
        move_selection_sprites(cursor.x, cursor.y);
        selection.x = cursor.x;
        selection.y = cursor.y;
        square_selected = true;
    }
    else if (square_selected && move_is_legal(cursor.y, cursor.x, selection.y, selection.x, false))
    {
        if (!replay_mode) save_move();

        event = move_piece_board(cursor.y, cursor.x, selection.y, selection.x, false);

        uint8_t piece = get_piece(selection.y, selection.x);
        move_piece_screen(cursor.x, cursor.y, selection.x, selection.y, piece);
        move_cursor_sprites(selection.x, selection.y);

        switch (event)
        {
        case Castles:
            move_piece_screen(7 * (selection.x == 6), cursor.y, 
                              (selection.x + cursor.x) / 2, selection.y,
                              Rook + 6 * get_color(piece));
            break;
        case Promotion:;
            handle_promotion();
            break;
        case En_passaint:;
            set_piece(selection.y + 1 - 2 * get_color(piece), selection.x, no_Piece);
            draw_blank_square(selection.x, selection.y + 1 - 2 * get_color(piece));
            break;
        case Checkmate:
            init_game();
            break;
        case Remis:
            init_game();
            break;
        }
        
        cursor.x = selection.x;
        cursor.y = selection.y;
        square_selected = false;
        hide_selection();
        player = !player;
    }
    else
    {
        square_selected = false;
        hide_selection();
    }
}


void replay()
{
    replay_mode = true;
    init_game();

    uint16_t current_move = 0;

    while (!(joypad_state & J_B))
    { 
        if (current_move >= num_moves)
        {
            replay_mode = false;
            return;
        }
        if (joypad_state & J_RIGHT)
        {
            cursor.x = list_of_moves[current_move][0];
            cursor.y = list_of_moves[current_move][1];
            selection.x = list_of_moves[current_move][2];
            selection.y = list_of_moves[current_move][3];

            square_selected = true;
            handle_button_a();

            current_move++;
            waitpadup();
        }
        joypad_state = joypad();
    }
    replay_mode = false;
}


void handle_button_select()
{
    while (joypad_state & J_SELECT)
    {
        if (joypad_state & J_A)
        {
            init_game();
        }
        else if (joypad_state & J_B)
        {
            init_game();
        }
        else if (joypad_state & J_START)
        {
            replay();
        }
        

        joypad_state = joypad();
    }
}


void handle_input()
{
    wait_vbl_done();
    joypad_state = joypad();

    if (joypad_state & DPAD_PRESSED)
    {
        handle_dpad(joypad_state);
    }
    else if (joypad_state & J_A)
    {
        handle_button_a();
    }
    else if (joypad_state & J_SELECT)
    {
        handle_button_select();
    }

    waitpadup();
}