#include "game.h"


void main(void)
{
    title_screen();

    init_game();

    while (1)
    {
        handle_input();
    }
}
