#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "chessboard.h"
#include "graphics.h"

const uint8_t DPAD_PRESSED = J_LEFT | J_RIGHT | J_UP | J_DOWN;

uint8_t joypad_state = 0;

bool square_selected = false;

bool player = 0;

uint8_t event = no_Event;

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


void init_game()
{
    init_cursor();
    init_graphics();
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
    else if (square_selected && move_is_legal(cursor.y, cursor.x, selection.y, selection.x))
    {
        event = move_piece_board(cursor.y, cursor.x, selection.y, selection.x);
        switch (event)
        {
        case Castles:
            break;
        case Promotion:;
            move_piece_screen(cursor.x, cursor.y, selection.x, selection.y, get_piece(selection.y, selection.x));
            move_cursor_sprites(selection.x, selection.y);
            set_piece(selection.y, selection.x, Queen + 6 * get_color(get_piece(selection.y, selection.x)));
            draw_piece(selection.x, selection.y, Queen + 6 * get_color(get_piece(selection.y, selection.x)));
            break;
        case En_passaint:;
            uint8_t piece = get_piece(selection.y, selection.x);
            set_piece(selection.y + 1 - 2 * get_color(piece), selection.x, no_Piece);
            draw_blank_square(selection.x, selection.y + 1 - 2 * get_color(piece));
            move_piece_screen(cursor.x, cursor.y, selection.x, selection.y, piece);
            move_cursor_sprites(selection.x, selection.y);
            break;
        case Checkmate:
            break;
        case Stalemate:
            break;
        default:
            move_piece_screen(cursor.x, cursor.y, selection.x, selection.y, get_piece(selection.y, selection.x));
            move_cursor_sprites(selection.x, selection.y);
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

    waitpadup();
}