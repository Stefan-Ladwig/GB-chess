#include <gb/gb.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../resources/chess_tilemap.h"
#include "../resources/chess_tiles.h"
#include "../resources/chess_sprite_tiles.h"


void init_background()
{
    set_bkg_data(0, 99, chess_tiles);
    set_bkg_tiles(0, 0, 20, 18, chess_tilemap);
    SHOW_BKG;
}


void init_sprites()
{
    SPRITES_8x8;
    
    set_sprite_data(0, 4, chess_sprite_tiles);

    for (uint8_t i = 0; i < 8; i++)
    {
        set_sprite_tile(i, i / 2);
    }

    for (uint8_t j = 0; j < 8; j++)
    {
        set_sprite_tile(j + 8, j / 2);
    }

    SHOW_SPRITES;
}


void init_graphics()
{
    disable_interrupts();
    DISPLAY_OFF;

    init_background();
    init_sprites();

    DISPLAY_ON;
    enable_interrupts();
}


void hide_cursor()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        move_sprite(i + 8, 0, 0);
    }
}


void hide_selection()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        move_sprite(i, 0, 0);
    }
}


void move_cursor_sprites(uint8_t x, uint8_t y)
{    
    move_sprite( 8,  8 + x * 16, 24 + y * 16);
    move_sprite( 9,  8 + x * 16, 32 + y * 16);
    move_sprite(10, 16 + x * 16, 40 + y * 16);
    move_sprite(11, 24 + x * 16, 40 + y * 16);
    move_sprite(12, 32 + x * 16, 32 + y * 16);
    move_sprite(13, 32 + x * 16, 24 + y * 16);
    move_sprite(14, 24 + x * 16, 16 + y * 16);
    move_sprite(15, 16 + x * 16, 16 + y * 16);
}


void move_selection_sprites(uint8_t x, uint8_t y)
{    
    move_sprite(0,  8 + x * 16, 24 + y * 16);
    move_sprite(1,  8 + x * 16, 32 + y * 16);
    move_sprite(2, 16 + x * 16, 40 + y * 16);
    move_sprite(3, 24 + x * 16, 40 + y * 16);
    move_sprite(4, 32 + x * 16, 32 + y * 16);
    move_sprite(5, 32 + x * 16, 24 + y * 16);
    move_sprite(6, 24 + x * 16, 16 + y * 16);
    move_sprite(7, 16 + x * 16, 16 + y * 16);
}


void draw_blank_square(uint8_t x, uint8_t y)
{
    bool square_color = (x + y) % 2;

    set_bkg_tile_xy(x * 2 + 1, y * 2 + 1, 97 + square_color);
    set_bkg_tile_xy(x * 2 + 2, y * 2 + 1, 97 + square_color);
    set_bkg_tile_xy(x * 2 + 1, y * 2 + 2, 97 + square_color);
    set_bkg_tile_xy(x * 2 + 2, y * 2 + 2, 97 + square_color); 
}


void draw_piece(uint8_t x, uint8_t y, uint8_t piece)
{
    bool square_color = (x + y) % 2;
    bool piece_color = (piece > 6);
    piece -= 6 * piece_color;
    uint8_t tile_nr = 48 * square_color + 24 * (1 - piece_color) + 2 * (piece - 1);

    set_bkg_tile_xy(x * 2 + 1, y * 2 + 1, tile_nr);
    set_bkg_tile_xy(x * 2 + 2, y * 2 + 1, tile_nr + 1);
    set_bkg_tile_xy(x * 2 + 1, y * 2 + 2, tile_nr + 12);
    set_bkg_tile_xy(x * 2 + 2, y * 2 + 2, tile_nr + 13);
}


void move_piece_screen(uint8_t origin_x, uint8_t origin_y, 
                uint8_t destination_x, uint8_t destination_y, uint8_t piece)
{
    draw_blank_square(origin_x, origin_y);
    draw_piece(destination_x, destination_y, piece);
}