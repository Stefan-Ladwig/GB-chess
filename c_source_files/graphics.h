#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

void init_graphics();

void move_selection_sprites(uint8_t x, uint8_t y);

void move_cursor_sprites(uint8_t x, uint8_t y);

void move_cursor_sprites_absolute(uint8_t x, uint8_t y);

void draw_blank_square(uint8_t x, uint8_t y);

void draw_piece(uint8_t x, uint8_t y, uint8_t piece);

void move_piece_screen(uint8_t origin_x, uint8_t origin_y, 
                       uint8_t destination_x, uint8_t destination_y, uint8_t piece);

void hide_cursor();

void hide_selection();

void show_endgame_screen(uint8_t ending_event, bool player, uint8_t *buffer);

void hide_endgame_screen(uint8_t ending_event, bool player, uint8_t *buffer);

void show_promotion_screen(bool player, uint8_t *buffer);

void hide_promotion_screen(bool player, uint8_t *buffer);

#endif