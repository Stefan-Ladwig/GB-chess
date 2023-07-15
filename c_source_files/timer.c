#include <stdint.h>
#include <gb/gb.h>
#include <stdbool.h>
#include "graphics.h"
#include "game.h"
#include "enums.h"

uint8_t timer_delay = 0;
uint16_t time_start = 0;
uint16_t time[2] = {0};
uint8_t bonus = 0;


uint16_t get_time_start()
{
    return time_start;
}


uint8_t get_bonus()
{
    return bonus;
}


void pause_timer()
{
    set_interrupts(VBL_IFLAG);
}


void start_timer()
{
    if (time_start == 0) return;

    set_interrupts(VBL_IFLAG | TIM_IFLAG);
}


void interruptTIM()
{
    timer_delay++;
    if (timer_delay < 16)
        return;
    
    time[player]--;
    update_timer(player, time[player]);
    timer_delay = 0;
    
    if (time[player] == 0)
        handle_endgame(Checkmate);
}


void init_timer()
{
    CRITICAL
    {
        TAC_REG = 0b100;
        TMA_REG = 0;
        remove_TIM(interruptTIM);
        add_TIM(interruptTIM);
    }
    pause_timer();

    if (time_start == 0)
    {
        hide_timer();
        return;
    }

    time[0] = time_start;
    time[1] = time_start;
    
    update_timer(0, time[0]);
    update_timer(1, time[1]);
}


void player_switched()
{
    if (time_start == 0) return;
    
    time[player] += bonus;
    update_timer(player, time[player]);
}


void add_to_timer(int8_t direction, uint8_t menu_item, uint8_t time_digit)
{
    uint16_t factor = 1;
    if (time_digit % 2 == 0)
        factor *= 10;

    if (menu_item == 0)
    {
        if (time_digit < 2)
            factor *= 60;
        
        if (direction < 0)
        {
            if (factor > time_start)
                time_start = 0;
            else
                time_start -= factor;
        }
        else if (direction > 0)
        {
            time_start = time_start + factor;

            if (time_start > 6000) time_start = 0;
        }
    }
    else if (menu_item == 1)
    {
        if (direction < 0)
        {
            if (factor > bonus)
                bonus = 0;
            else
                bonus -= factor;
        }
        else if (direction > 0)
        {
            bonus = bonus + factor;

            if (bonus > 100) bonus = 0;
        }
    }
    update_timer_settings(time_start, bonus);
}