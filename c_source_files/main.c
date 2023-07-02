#include <gb/gb.h>
#include <stdint.h>
#include <gb/hardware.h>
#include "../resources/chess_tilemap.h"
#include "../resources/chess_tiles.h"


int main(void)
{
    disable_interrupts();
    DISPLAY_OFF;
    
    set_bkg_data(0, 99, chess_tiles);
    set_bkg_tiles(0, 0, 20, 18, chess_tilemap);
    SHOW_BKG;

    DISPLAY_ON;
    enable_interrupts();
    return 0;
}
