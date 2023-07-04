#include "game.h"


void main(void)
{
    init_game();

    while (1)
    {
        handle_input();
    }
}
