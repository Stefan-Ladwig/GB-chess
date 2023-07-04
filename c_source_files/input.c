#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "chessboard.h"
#include "graphics.h"

const uint8_t DPAD_PRESSED = J_LEFT | J_RIGHT | J_UP | J_DOWN;

uint8_t joypad_state = 0;

bool square_selected = false;

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


int8_t *get_direction(uint8_t joypad_state)
{
    int8_t *direction = malloc(2 * sizeof(int8_t));

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
    return direction;
}


void move_cursor(uint8_t joypad_state)
{    
    int8_t *dir = get_direction(joypad_state);

    cursor.x = (cursor.x + dir[0] + 8) % 8;
    cursor.y = (cursor.y + dir[1] + 8) % 8;

    free(dir);
}


void move_selection(uint8_t joypad_state)
{    
    int8_t *dir = get_direction(joypad_state);

    selection.x = (selection.x + dir[0] + 8) % 8;
    selection.y = (selection.y + dir[1] + 8) % 8;

    free(dir);
}


void handle_input()
{
        wait_vbl_done();
        joypad_state = joypad();

        if (joypad_state & DPAD_PRESSED)
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
        else if (joypad_state & J_A)
        {
            if (!square_selected && piece_on_square(cursor.y, cursor.x))
            {
                move_selection_sprites(cursor.x, cursor.y);
                selection.x = cursor.x;
                selection.y = cursor.y;
                square_selected = true;
            }
            else if (square_selected && move_is_legal(cursor.y, cursor.x, selection.y, selection.x))
            {
                move_cursor_sprites(selection.x, selection.y);
                cursor.x = selection.x;
                cursor.y = selection.y;
                square_selected = false;
                hide_selection();
            }
            else
            {
                square_selected = false;
                hide_selection();
            }
        }

        waitpadup();
}