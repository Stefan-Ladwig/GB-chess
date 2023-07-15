#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "chessboard.h"
#include "graphics.h"
#include "timer.h"
#include "enums.h"

const uint8_t DPAD_PRESSED = J_LEFT | J_RIGHT | J_UP | J_DOWN;

uint8_t joypad_state;
bool square_selected;
bool player;
bool replay_mode = false;
uint8_t event;
uint8_t **list_of_moves;
uint16_t num_moves;
uint8_t *tile_buffer = (uint8_t*) NULL;
uint8_t pawn_promotion[2][8] = {{no_Piece}};

struct pos {
    uint8_t x;
    uint8_t y;
} cursor, selection;


void init_cursor()
{
    selection.x = 0;
    selection.y = 0;
    
    cursor.x = 3;
    cursor.y = 6;

    move_cursor_sprites(cursor.x, cursor.y);
}


void init_list_of_moves()
{
    for (uint8_t i = 0; i < num_moves; i++)
    {
        free(list_of_moves[i]);
    }    

    num_moves = 0;
    list_of_moves = realloc(list_of_moves, 0);
}


void handle_clock_menu();
void handle_start();


void init_game()
{
    pause_timer();

    init_board();
    init_graphics();
    HIDE_SPRITES;

    if (!replay_mode) handle_start();

    SHOW_SPRITES;
    init_cursor();
    init_timer();
    if (!replay_mode) init_list_of_moves();

    joypad_state = 0;
    square_selected = false;
    player = false;
    event = no_Event;
}


void save_move()
{
    num_moves++;
    list_of_moves = realloc(list_of_moves, num_moves * sizeof(uint8_t*));

    uint8_t *move = malloc(4 * sizeof(uint8_t));

    move[0] = cursor.x;
    move[1] = cursor.y;
    move[2] = selection.x;
    move[3] = selection.y;

    list_of_moves[num_moves - 1] = move;
}


void get_direction(uint8_t joypad_state, int8_t direction[2])
{
    switch (joypad_state)
    {
    case J_LEFT:
        direction[0] = -1;
        break;

    case J_RIGHT:
        direction[0] = 1;
        break;

    case J_UP:
        direction[1] = -1;
        break;

    case J_DOWN:
       direction[1] = 1;
        break;
    }
}


void move_cursor(uint8_t joypad_state)
{    
    int8_t dir[2] = {0};
    get_direction(joypad_state, dir);
    cursor.x = (cursor.x + dir[0] + 8) % 8;
    cursor.y = (cursor.y + dir[1] + 8) % 8;
}


void move_selection(uint8_t joypad_state)
{    
    int8_t dir[2] = {0};
    get_direction(joypad_state, dir);

    selection.x = (selection.x + dir[0] + 8) % 8;
    selection.y = (selection.y + dir[1] + 8) % 8;
}


void handle_dpad(uint8_t joypad_state)
{
    if (square_selected)
    {
        move_selection(joypad_state);
        move_selection_sprites(selection.x, selection.y);
    }
    else
    {
        move_cursor(joypad_state);
        move_cursor_sprites(cursor.x, cursor.y);
    }
}


void handle_promotion()
{
    uint8_t piece = no_Piece;

    if (replay_mode)
        piece = pawn_promotion[player][selection.x];
    else
    {
        piece = Queen;
        uint8_t *tile_buffer = malloc(4 * 16);
        show_promotion_screen(player, tile_buffer);

        hide_selection();
        uint8_t y_screen = 24 + player * 12 * 8;
        move_cursor_sprites_absolute(16, y_screen);

        waitpadup();
        joypad_state = joypad();

        while (!(joypad_state & J_A))
        {
            if (joypad_state & J_LEFT || joypad_state & J_RIGHT)
            {
                if (joypad_state & J_RIGHT) piece++;
                else if (joypad_state & J_LEFT) piece--;
                piece = (piece - 2 + 4) % 4 + 2;

                move_cursor_sprites_absolute(16 + (piece - 2) * 32, y_screen);

                waitpadup();
            }
            joypad_state = joypad();
        }
        hide_promotion_screen(player, tile_buffer);
        move_cursor_sprites(selection.x, selection.y);
        pawn_promotion[player][selection.x] = piece;
    }
    set_piece(selection.y, selection.x, piece + 6 * player);
    draw_piece(selection.x, selection.y, piece + 6 * player);
}


void replay();


void handle_endgame(uint8_t event)
{
    HIDE_SPRITES;
    pause_timer();
    uint8_t *tile_buffer = malloc(16 * 3);
    show_endgame_screen(event, player, tile_buffer);
    waitpadup();
    joypad_state = joypad();

    while (!joypad_state)
    {
        joypad_state = joypad();
    }
    
    hide_endgame_screen(event, player, tile_buffer);
    free(tile_buffer);

    while (joypad_state & J_SELECT)
    {
        if(joypad_state & J_START)
        {
            replay();
            return;
        }
        joypad_state = joypad();
    }
    init_game();
}


void handle_button_a()
{
    if (!square_selected && piece_on_square(cursor.y, cursor.x) &&
        player == get_color(get_piece(cursor.y, cursor.x)))
    {
        move_selection_sprites(cursor.x, cursor.y);
        selection.x = cursor.x;
        selection.y = cursor.y;
        square_selected = true;
    }
    else if (square_selected && move_is_legal(cursor.y, cursor.x, selection.y, selection.x, false))
    {
        if (num_moves == 0) start_timer();
        if (!replay_mode) save_move();

        event = move_piece_board(cursor.y, cursor.x, selection.y, selection.x, false);

        uint8_t piece = get_piece(selection.y, selection.x);
        move_piece_screen(cursor.x, cursor.y, selection.x, selection.y, piece);
        move_cursor_sprites(selection.x, selection.y);

        switch (event)
        {
        case Castles:
            move_piece_screen(7 * (selection.x == 6), cursor.y, 
                              (selection.x + cursor.x) / 2, selection.y,
                              Rook + 6 * get_color(piece));
            break;
        case Promotion:;
            handle_promotion();
            break;
        case En_passaint:;
            set_piece(selection.y + 1 - 2 * get_color(piece), selection.x, no_Piece);
            draw_blank_square(selection.x, selection.y + 1 - 2 * get_color(piece));
            break;
        case Checkmate:
            handle_endgame(Checkmate);
            break;
        case Remis:
            handle_endgame(Remis);
            break;
        }
        
        cursor.x = selection.x;
        cursor.y = selection.y;
        square_selected = false;
        hide_selection();
        player_switched();
        player = !player;
    }
    else
    {
        square_selected = false;
        hide_selection();
    }
}


void replay()
{
    replay_mode = true;
    init_game();

    uint16_t current_move = 0;

    while (!(joypad_state & J_B))
    { 
        if (current_move >= num_moves)
        {
            replay_mode = false;
            return;
        }
        if (joypad_state & J_RIGHT)
        {
            cursor.x = list_of_moves[current_move][0];
            cursor.y = list_of_moves[current_move][1];
            selection.x = list_of_moves[current_move][2];
            selection.y = list_of_moves[current_move][3];

            square_selected = true;
            handle_button_a();

            current_move++;
            waitpadup();
        }
        joypad_state = joypad();
    }
    num_moves = current_move;
    replay_mode = false;
}


void handle_button_select()
{
    while (joypad_state & J_SELECT)
    {
        if (joypad_state & J_A)
        {
            handle_endgame(Remis);
        }
        else if (joypad_state & J_B)
        {
            handle_endgame(Checkmate);
        }
        else if (joypad_state & J_START)
        {
            replay();
        }

        joypad_state = joypad();
    }
}


void handle_input()
{
    wait_vbl_done();
    joypad_state = joypad();

    if (joypad_state == 0x00) return;

    if (joypad_state & DPAD_PRESSED)
    {
        handle_dpad(joypad_state);
    }
    else if (joypad_state & J_A)
    {
        handle_button_a();
    }
    else if (joypad_state & J_SELECT)
    {
        handle_button_select();
    }
    waitpadup();
}


void handle_clock_menu()
{
    uint8_t menu_item = 0;
    bool option_selected = 0;
    int8_t time_digit = 0;
    int8_t direction = 0;

    show_menu();
    update_timer_settings(get_time_start(), get_bonus());
    move_menu_arrow(menu_item);

    while (1)
    {
        waitpadup();
        joypad_state = joypad();

        while (!joypad_state)
        {
            joypad_state = joypad();
        }

        if (option_selected)
        {
            if (joypad_state & J_B)
            {
                option_selected = false;
                clear_digit_arrow(menu_item);
                time_digit = 0;
            }
            else if ((joypad_state & J_LEFT) || (joypad_state & J_RIGHT))
            {
                if (joypad_state & J_LEFT)
                    time_digit--;
                else if (joypad_state & J_RIGHT)
                    time_digit++;

                time_digit = (time_digit + 4) % 4;
                if (menu_item == 1) time_digit %= 2;
                move_digit_arrow(time_digit, menu_item);
            }
            else if ((joypad_state & J_DOWN) || (joypad_state & J_UP))
            {
                direction = (joypad_state & J_UP) - 2 * (joypad_state & J_DOWN);
                add_to_timer(direction, menu_item, time_digit);
            }
        }
        else if (!option_selected)
        {
            if (joypad_state & J_B)
                break;
            
            if (joypad_state & J_A)
            {
                option_selected = true;
                move_digit_arrow(time_digit, menu_item);
            }
            else if ((joypad_state & J_DOWN) || (joypad_state & J_UP))
            {
                menu_item = (menu_item + 1 + 2) % 2;
                move_menu_arrow(menu_item);
            }
        }
        waitpadup();
    }
    waitpadup();
}


void handle_start()
{
    uint8_t menu_item = 0;

    show_start();
    move_menu_arrow(menu_item);
    waitpadup();

    while (1)
    {
        joypad_state = joypad();

        while (!joypad_state)
        {
            joypad_state = joypad();
        }

        if ((menu_item == 0) && (joypad_state & J_A))
            break;
        
        if ((menu_item == 1) && (joypad_state & J_A))
        {
            handle_clock_menu();
            show_start();
            menu_item = 0;
            move_menu_arrow(menu_item);
        }
        else if ((joypad_state & J_DOWN) || (joypad_state & J_UP))
        {
            menu_item = (menu_item + 1 + 2) % 2;
            move_menu_arrow(menu_item);
        }
        waitpadup();
    }
    waitpadup();
    hide_start();
}