#include <gb/hardware.h>
#include <gb/gb.h>
#include <stdint.h>
#include "chessboard.h"
#include "graphics.h"

const uint8_t DPAD_PRESSED = J_LEFT | J_RIGHT | J_UP | J_DOWN;

uint8_t joypad_state = 0;

struct cursor {
    uint8_t x;
    uint8_t y;
} cursor;


void init_cursor()
{
    cursor.x = 3;
    cursor.y = 6;

    move_selection_sprites(cursor.x, cursor.y);
}


void move_cursor(uint8_t joypad_state)
{    
    int8_t x = 0, y = 0;

    switch (joypad_state)
    {
    case J_LEFT:
        x = -1;
        break;

    case J_RIGHT:
        x = 1;
        break;

    case J_UP:
        y = -1;
        break;

    case J_DOWN:
        y = 1;
        break;
    
    default:
        return;
    }

    cursor.x = (cursor.x + x + 8) % 8;
    cursor.y = (cursor.y + y + 8) % 8;
}


void handle_input()
{
        wait_vbl_done();
        joypad_state = joypad();

        if (joypad_state & DPAD_PRESSED)
        {
            move_cursor(joypad_state);
            move_selection_sprites(cursor.x, cursor.y);
        }

        waitpadup();
}