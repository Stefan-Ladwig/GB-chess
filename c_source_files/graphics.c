#include <gb/gb.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "chessboard.h"
#include "../resources/chess_tilemap.h"
#include "../resources/chess_tiles.h"
#include "../resources/chess_sprite_tiles.h"
#include "../resources/chess_window_tilemap.h"


void init_background()
{
    set_bkg_data(0, 167, chess_tiles);
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

    HIDE_WIN;

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


void move_cursor_sprites_absolute(uint8_t x, uint8_t y)
{    
    move_sprite( 8,      x,  8 + y);
    move_sprite( 9,      x, 16 + y);
    move_sprite(10,  8 + x, 24 + y);
    move_sprite(11, 16 + x, 24 + y);
    move_sprite(12, 24 + x, 16 + y);
    move_sprite(13, 24 + x,  8 + y);
    move_sprite(14, 16 + x,      y);
    move_sprite(15,  8 + x,      y);
}


void move_cursor_sprites(uint8_t x, uint8_t y)
{    
    move_cursor_sprites_absolute(16 * x + 8, 16 * y + 16);
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


uint8_t get_row_endgame_window(uint8_t event, bool player)
{
    uint8_t row = 0;

    if (event == Remis)
        row = 7;
    else if (event == Checkmate)
        row = 5 + player * 5;

    return row;
}


void show_endgame_screen(uint8_t ending_event, bool player, uint8_t *buffer)
{
    uint8_t row = get_row_endgame_window(ending_event, player);

    get_bkg_tiles(1, row, 16, 3, buffer);
    set_bkg_tiles(1, row, 16, 3, chess_window_tilemap + (16 * (row - 1 - player * (ending_event != Remis))));
}


void hide_endgame_screen(uint8_t ending_event, bool player, uint8_t *buffer)
{
    uint8_t row = get_row_endgame_window(ending_event, player);
    
    set_bkg_tiles(1, row, 16, 3, buffer);
}


void show_promotion_screen(bool player, uint8_t *buffer)
{
    uint8_t row = 1 + player * 12;

    get_bkg_tiles(1, row, 16, 4, buffer);
    set_bkg_tiles(1, row, 16, 4, chess_window_tilemap + (16 * (row - 1 - player)));
}


void hide_promotion_screen(bool player, uint8_t *buffer)
{
    uint8_t row = 1 + player * 12;

    set_bkg_tiles(1, row, 16, 4, buffer);
}