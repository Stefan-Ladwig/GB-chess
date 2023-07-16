#include <gb/gb.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "enums.h"
#include "../resources/chess_tilemap.h"
#include "../resources/chess_tiles.h"
#include "../resources/chess_sprite_tiles.h"
#include "../resources/chess_window_tilemap.h"
#include "../resources/chess_menu_tilemap.h"
#include "../resources/chess_start_tilemap.h"
#include "../resources/chess_logo_tiles.h"
#include "../resources/chess_logo_tilemap.h"
#include "../resources/chess_filelabel_tilemap.h"
#include "../resources/chess_ranklabel_tilemap.h"
#include "../resources/chess_replay_label_tilemap.h"


void show_logo()
{
    set_bkg_1bpp_data(176, 55, chess_logo_tiles);
    fill_bkg_rect(0, 0, 20, 18, 176);
    set_bkg_tiles(7, 3, 6, 13, chess_logo_tilemap);
    SHOW_BKG;
}


void init_background()
{
    set_bkg_data(0, 208, chess_tiles);
    set_bkg_tiles(0, 0, 20, 18, chess_tilemap);
    SHOW_BKG;
}


void init_sprites()
{
    SPRITES_8x8;
    
    set_sprite_data(0, 5, chess_sprite_tiles);

    for (uint8_t i = 0; i < 8; i++)
    {
        set_sprite_tile(i, i / 2);
    }

    for (uint8_t j = 0; j < 8; j++)
    {
        set_sprite_tile(j + 8, j / 2);
    }

    set_sprite_tile(16, 4);

    SHOW_SPRITES;
}


void init_graphics()
{
    disable_interrupts();
    DISPLAY_OFF;

    init_background();
    init_sprites();
    move_sprite(16, 152, 83);
    set_sprite_prop(16, 0);

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


void update_timer(bool player, uint16_t time)
{
    uint8_t minutes = time / 60;
    uint8_t seconds = time % 60;

    set_bkg_tile_xy(18, 10 - player * 7, 128 + minutes / 10);
    set_bkg_tile_xy(18, 11 - player * 7, 128 + minutes % 10);
    set_bkg_tile_xy(18, 13 - player * 7, 128 + seconds / 10);
    set_bkg_tile_xy(18, 14 - player * 7, 128 + seconds % 10);
}


void hide_timer()
{
    fill_bkg_rect(18, 3, 1, 12, 99);
}


void update_timer_settings(uint16_t time, uint8_t bonus)
{
    uint8_t minutes = time / 60;
    uint8_t seconds = time % 60;

    set_bkg_tile_xy(10, 7, 100 + minutes / 10);
    set_bkg_tile_xy(11, 7, 100 + minutes % 10);
    set_bkg_tile_xy(13, 7, 100 + seconds / 10);
    set_bkg_tile_xy(14, 7, 100 + seconds % 10);

    seconds = bonus;

    set_bkg_tile_xy(13, 10, 100 + seconds / 10);
    set_bkg_tile_xy(14, 10, 100 + seconds % 10);
}


void show_menu()
{
    set_bkg_tiles(1, 5, 16, 8, chess_menu_tilemap);
}


void hide_menu()
{
    set_bkg_submap(1, 5, 16, 8, chess_tilemap, 20);
}


void show_start()
{
    set_bkg_tiles(1, 5, 16, 8, chess_start_tilemap);
}


void hide_start()
{
    set_bkg_submap(1, 5, 16, 8, chess_tilemap, 20);
}


uint8_t get_digit_arrow_y(uint8_t menu_item)
{
    return 8 + 3 * (menu_item == 1);
}


void clear_digit_arrow(uint8_t menu_item)
{
    uint8_t y = get_digit_arrow_y(menu_item);

    fill_bkg_rect(10, y, 5, 1, 96);
}


void move_digit_arrow(uint8_t time_digit, uint8_t menu_item)
{
    uint8_t x = 13 + time_digit;
    if (menu_item == 0)
        x = x - 3 + (time_digit >= 2);
    uint8_t y = get_digit_arrow_y(menu_item);

    fill_bkg_rect(10, y, 5, 1, 96);
    set_bkg_tile_xy(x, y, 112);
}


void move_menu_arrow(uint8_t menu_item)
{
    fill_bkg_rect(3, 6, 1, 6, 96);
    set_bkg_tile_xy(3, 7 + 3 * (menu_item == 1), 111);
}


void show_rank_labels()
{
    set_bkg_tiles(0, 1, 1, 16, chess_ranklabel_tilemap);
}


void show_file_labels()
{
    set_bkg_tiles(1, 17, 16, 1, chess_filelabel_tilemap);
}


void show_labels()
{
    show_rank_labels();
    show_file_labels();
}


void hide_rank_labels()
{
    fill_bkg_rect(0, 1, 1, 16, 99);
}


void hide_file_labels()
{
    fill_bkg_rect(1, 17, 16, 1, 99);
}


void hide_labels()
{
    hide_rank_labels();
    hide_file_labels();
}


void flip_player_indicator()
{
    set_sprite_prop(16,get_sprite_prop(16) ^ 0b01000000);
}


void show_replay_label()
{
    set_bkg_tiles(3, 0, 12, 1, chess_replay_label_tilemap);
}


void hide_replay_label()
{
    fill_bkg_rect(3, 0, 12, 1, 99);
}