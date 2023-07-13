#include <stdint.h>
#include <gb/gb.h>
#include <stdbool.h>
#include "graphics.h"
#include "game.h"
#include "enums.h"

uint8_t timer_delay = 0;
uint16_t time[2] = {0};


void pause_timer()
{
    set_interrupts(VBL_IFLAG);
}


void start_timer()
{
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
    
    if (time[player] == 0) handle_endgame(Checkmate);
}


void init_timer()
{
    CRITICAL
    {
        TAC_REG = 0b100;
        TMA_REG = 0;
        add_TIM(interruptTIM);
    }
    pause_timer();

    time[0] = 10;
    time[1] = 10;
    
    update_timer(0, time[0]);
    update_timer(1, time[1]);
}