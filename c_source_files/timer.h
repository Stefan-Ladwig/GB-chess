#ifndef TIMER_H
#define TIMER_H

uint16_t get_time_start();

uint8_t get_bonus();

void init_timer();

void start_timer();

void pause_timer();

void player_switched();

void add_to_timer(int8_t direction, uint8_t menu_item, uint8_t time_digit);

#endif