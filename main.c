#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "decl.h"
#include "init.h"
#include "game.h"

int main(int argc, char **argv)
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *hex = NULL;
    ALLEGRO_BITMAP *rose = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_FONT *big_font = NULL;
    //game states
    bool in_menu=true, in_editor=false, choosing_lvl=false, playing_lvl=false, in_options=false, in_credits=false;
    bool valid_lvl=true;
    bool redraw = true;
    bool doexit = false;
    bool lvl_won=false;
    struct pos PlayerPos,CursorPos;
    PlayerPos.x=-1;                                                 //initial (null) position of the player
    PlayerPos.y=-1;
    CursorPos.x=7;
    CursorPos.y=5;
    bool key[21] = {false, false, false, false, false, false,
                    false, false, false, false, false, false,
                    false, false, false, false, false, false,
                    false, false, false};                          //keeps track of what keys are pressed
    double last_key_time=0;                                        //keeps track of when the last move occurred
    int crates=0, targets=0, crates_on_targets=0;                  //keeps track of completing a level

    //handling level selection
    int number_of_current_lvl_set_selected=0, number_of_current_lvl_selected, number_of_lvls_in_set;
    char current_lvl_set_selected_name[20];
    int number_of_lvl_sets=0;
    int number_of_moves;

    //handling the menu
    int menu_position=0;
    const int number_of_menu_positions=5;
    int options_position=0;
    const int number_of_options_positions=2;

    int move_horizontal;                //handles moving horizontally between stuff in options

    //creating the board array
    struct hex board[BoardW][BoardH+1];


    if(init(&display, &event_queue, &timer)==-1)return -1;
    if(load_defaults(&font, &big_font, &hex, &rose)==-1)return -1;

    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);

    //main loop
    while(!doexit)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            if(in_menu)
            {
                if(key[UP] && al_get_time()-last_key_time>min_time && menu_position>0){menu_position--;last_key_time=al_get_time();}
                if(key[DOWN] && al_get_time()-last_key_time>min_time && menu_position<number_of_menu_positions-1){menu_position++;last_key_time=al_get_time();}

                if(key[ENTER] && al_get_time()-last_key_time>min_time)
                {
                    last_key_time=al_get_time();
                    if(menu_position==0){in_menu=false; choosing_lvl=true;}
                    else if(menu_position==1)
                    {
                        number_of_current_lvl_selected=0;
                        in_menu=false;
                        in_editor=true;
                        read_number_of_lvls(&number_of_lvls_in_set, "custom");
                        board_load(board, "custom", number_of_current_lvl_selected, &crates, &targets, &crates_on_targets, &number_of_moves, &valid_lvl, true, &lvl_won, &PlayerPos);
                    }
                    else if(menu_position==2)
                    {
                        options_position=0;
                        in_menu=false;
                        in_options=true;
                    }
                    else if(menu_position==3)
                    {
                        in_menu=false;
                        in_credits=true;
                    }
                    else if(menu_position==4)doexit=true;
                }
                if(key[ESC] && al_get_time()-last_key_time>3*min_time)doexit=true;
            }
            else if(in_editor)
            {
                if(key[Q] && al_get_time()-last_key_time>min_time)if(editor_function('q', board, &CursorPos)==1)last_key_time=al_get_time();
                if(key[W] && al_get_time()-last_key_time>min_time)if(editor_function('w', board, &CursorPos)==1)last_key_time=al_get_time();
                if(key[E] && al_get_time()-last_key_time>min_time)if(editor_function('e', board, &CursorPos)==1)last_key_time=al_get_time();
                if(key[A] && al_get_time()-last_key_time>min_time)if(editor_function('a', board, &CursorPos)==1)last_key_time=al_get_time();
                if(key[S] && al_get_time()-last_key_time>min_time)if(editor_function('s', board, &CursorPos)==1)last_key_time=al_get_time();
                if(key[D] && al_get_time()-last_key_time>min_time)if(editor_function('d', board, &CursorPos)==1)last_key_time=al_get_time();
                if(key[MINUS])editor_function('-', board, &CursorPos);
                if(key[O])editor_function('o', board, &CursorPos);
                if(key[P])editor_function('p', board, &CursorPos);
                if(key[X])editor_function('x', board, &CursorPos);
                if(key[C])editor_function('c', board, &CursorPos);
                if(key[T])editor_function('t', board, &CursorPos);

                if(key[R]){board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);last_key_time=al_get_time();}

                if(key[PGUP] && al_get_time()-last_key_time>min_time && number_of_current_lvl_selected>0){board_unload(board, "custom", number_of_current_lvl_selected);
                                                    number_of_current_lvl_selected--;last_key_time=al_get_time();
                                                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);
                                                    board_load(board, "custom", number_of_current_lvl_selected, &crates, &targets, &crates_on_targets, &number_of_moves, &valid_lvl, true, &lvl_won, &PlayerPos);}
                if(key[PGDOWN] && al_get_time()-last_key_time>min_time && number_of_current_lvl_selected<number_of_lvls_in_set-1){board_unload(board, "custom", number_of_current_lvl_selected);
                                                    number_of_current_lvl_selected++;last_key_time=al_get_time();
                                                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);
                                                    board_load(board, "custom", number_of_current_lvl_selected, &crates, &targets, &crates_on_targets, &number_of_moves, &valid_lvl, true, &lvl_won, &PlayerPos);}
                if(key[PGDOWN] && al_get_time()-last_key_time>min_time){board_unload(board, "custom", number_of_current_lvl_selected);
                                                    number_of_lvls_in_set++;number_of_current_lvl_selected++;last_key_time=al_get_time();
                                                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);}


                if(key[ESC] && al_get_time()-last_key_time>min_time)
                {
                    board_unload(board, "custom", number_of_current_lvl_selected);
                    write_number_of_lvls(&number_of_lvls_in_set, "custom");
                    last_key_time=al_get_time();
                    in_menu=true;
                    in_editor=false;

                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);
                }
            }
            else if(choosing_lvl)
            {
                if(key[UP] && al_get_time()-last_key_time>min_time && number_of_current_lvl_set_selected>0){number_of_current_lvl_set_selected--;last_key_time=al_get_time();}
                if(key[DOWN] && al_get_time()-last_key_time>min_time && number_of_current_lvl_set_selected<number_of_lvl_sets-1){number_of_current_lvl_set_selected++;last_key_time=al_get_time();}

                if(key[ENTER] && al_get_time()-last_key_time>min_time)
                {
                    last_key_time=al_get_time();

                    choosing_lvl=false;
                    playing_lvl=true;
                    number_of_current_lvl_selected=0;

                    read_number_of_lvls(&number_of_lvls_in_set, current_lvl_set_selected_name);
                    board_load(board, current_lvl_set_selected_name, number_of_current_lvl_selected, &crates, &targets, &crates_on_targets, &number_of_moves, &valid_lvl, false, &lvl_won, &PlayerPos);
                }

                if(key[ESC] && al_get_time()-last_key_time>min_time)
                {
                    last_key_time=al_get_time();
                    in_menu=true;
                    choosing_lvl=false;
                }
            }
            else if(playing_lvl){
                if(key[Q] && al_get_time()-last_key_time>min_time && !lvl_won)if(move('q', board, &crates_on_targets, &number_of_moves, targets, &lvl_won, &PlayerPos)==1)last_key_time=al_get_time();
                if(key[W] && al_get_time()-last_key_time>min_time && !lvl_won)if(move('w', board, &crates_on_targets, &number_of_moves, targets, &lvl_won, &PlayerPos)==1)last_key_time=al_get_time();
                if(key[E] && al_get_time()-last_key_time>min_time && !lvl_won)if(move('e', board, &crates_on_targets, &number_of_moves, targets, &lvl_won, &PlayerPos)==1)last_key_time=al_get_time();
                if(key[A] && al_get_time()-last_key_time>min_time && !lvl_won)if(move('a', board, &crates_on_targets, &number_of_moves, targets, &lvl_won, &PlayerPos)==1)last_key_time=al_get_time();
                if(key[S] && al_get_time()-last_key_time>min_time && !lvl_won)if(move('s', board, &crates_on_targets, &number_of_moves, targets, &lvl_won, &PlayerPos)==1)last_key_time=al_get_time();
                if(key[D] && al_get_time()-last_key_time>min_time && !lvl_won)if(move('d', board, &crates_on_targets, &number_of_moves, targets, &lvl_won, &PlayerPos)==1)last_key_time=al_get_time();

                if(key[R] && al_get_time()-last_key_time>min_time){board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);
                                                    board_load(board, current_lvl_set_selected_name, number_of_current_lvl_selected, &crates, &targets, &crates_on_targets, &number_of_moves, &valid_lvl, false, &lvl_won, &PlayerPos);}

                if(key[PGUP] && al_get_time()-last_key_time>min_time && number_of_current_lvl_selected>0){number_of_current_lvl_selected--;last_key_time=al_get_time();
                                                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);
                                                    board_load(board, current_lvl_set_selected_name, number_of_current_lvl_selected, &crates, &targets, &crates_on_targets, &number_of_moves, &valid_lvl, false, &lvl_won, &PlayerPos);}
                if(key[PGDOWN] && al_get_time()-last_key_time>min_time && number_of_current_lvl_selected<number_of_lvls_in_set-1){number_of_current_lvl_selected++;last_key_time=al_get_time();
                                                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);
                                                    board_load(board, current_lvl_set_selected_name, number_of_current_lvl_selected, &crates, &targets, &crates_on_targets, &number_of_moves, &valid_lvl, false, &lvl_won, &PlayerPos);}

                if(key[ESC] && al_get_time()-last_key_time>min_time)
                {
                    last_key_time=al_get_time();
                    choosing_lvl=true;
                    playing_lvl=false;

                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves, &lvl_won, &PlayerPos);
                }
            }
            else if(in_options)
            {
                move_horizontal=0;

                if(key[UP] && al_get_time()-last_key_time>min_time && options_position>0){options_position--;last_key_time=al_get_time();}
                if(key[DOWN] && al_get_time()-last_key_time>min_time && options_position<number_of_options_positions-1){options_position++;last_key_time=al_get_time();}

                if(key[LEFT] && al_get_time()-last_key_time>min_time){move_horizontal=-1;last_key_time=al_get_time();}
                if(key[RIGHT] && al_get_time()-last_key_time>min_time){move_horizontal=1;last_key_time=al_get_time();}

                if(key[ESC] && al_get_time()-last_key_time>min_time)
                {
                    last_key_time=al_get_time();
                    in_options=false;
                    in_menu=true;
                }
            }
            else if(in_credits)
            {
                if(key[ESC] && al_get_time()-last_key_time>min_time)
                {
                    last_key_time=al_get_time();
                    in_credits=false;
                    in_menu=true;
                }
            }

            redraw=true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)                  //updating what keys are pressed
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_Q:
                key[Q]=true;
                break;

            case ALLEGRO_KEY_W:
                key[W]=true;
                break;

            case ALLEGRO_KEY_E:
                key[E]=true;
                break;

            case ALLEGRO_KEY_A:
                key[A]=true;
                break;

            case ALLEGRO_KEY_S:
                key[S]=true;
                break;

            case ALLEGRO_KEY_D:
                key[D]=true;
                break;

            case ALLEGRO_KEY_UP:
                key[UP]=true;
                break;

            case ALLEGRO_KEY_DOWN:
                key[DOWN]=true;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[RIGHT]=true;
                break;

            case ALLEGRO_KEY_LEFT:
                key[LEFT]=true;
                break;

            case ALLEGRO_KEY_PGUP:
                key[PGUP]=true;
                break;

            case ALLEGRO_KEY_PGDN:
                key[PGDOWN]=true;
                break;

            case ALLEGRO_KEY_ENTER:
                key[ENTER]=true;
                break;

            case ALLEGRO_KEY_ESCAPE:
                key[ESC]=true;
                break;

            case ALLEGRO_KEY_MINUS:
            case ALLEGRO_KEY_PAD_MINUS:
                key[MINUS]=true;
                break;

            case ALLEGRO_KEY_O:
            case ALLEGRO_KEY_0:
                key[O]=true;
                break;

            case ALLEGRO_KEY_P:
                key[P]=true;
                break;

            case ALLEGRO_KEY_X:
                key[X]=true;
                break;

            case ALLEGRO_KEY_C:
                key[C]=true;
                break;

            case ALLEGRO_KEY_T:
                key[T]=true;
                break;

            case ALLEGRO_KEY_R:
                key[R]=true;
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)                    //updating what keys are released
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_Q:
                key[Q]=false;
                break;

            case ALLEGRO_KEY_W:
                key[W]=false;
                break;

            case ALLEGRO_KEY_E:
                key[E]=false;
                break;

            case ALLEGRO_KEY_A:
                key[A]=false;
                break;

            case ALLEGRO_KEY_S:
                key[S]=false;
                break;

            case ALLEGRO_KEY_D:
                key[D]=false;
                break;

            case ALLEGRO_KEY_UP:
                key[UP]=false;
                break;

            case ALLEGRO_KEY_DOWN:
                key[DOWN]=false;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[RIGHT]=false;
                break;

            case ALLEGRO_KEY_LEFT:
                key[LEFT]=false;
                break;

            case ALLEGRO_KEY_PGUP:
                key[PGUP]=false;
                break;

            case ALLEGRO_KEY_PGDN:
                key[PGDOWN]=false;
                break;

            case ALLEGRO_KEY_ENTER:
                key[ENTER]=false;
                break;

            case ALLEGRO_KEY_ESCAPE:
                key[ESC]=false;
                break;

            case ALLEGRO_KEY_MINUS:
            case ALLEGRO_KEY_PAD_MINUS:
                key[MINUS]=false;
                break;

            case ALLEGRO_KEY_O:
            case ALLEGRO_KEY_0:
                key[O]=false;
                break;

            case ALLEGRO_KEY_P:
                key[P]=false;
                break;

            case ALLEGRO_KEY_X:
                key[X]=false;
                break;

            case ALLEGRO_KEY_C:
                key[C]=false;
                break;

            case ALLEGRO_KEY_T:
                key[T]=false;
                break;

            case ALLEGRO_KEY_R:
                key[R]=false;
                break;
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw=false;
            al_clear_to_color(al_map_rgb(255,255,255));
            if(in_menu)
            {
                draw_menu(font, big_font, menu_position);
            }

            else if(in_editor)
            {
                draw_lvl(board, hex, true, CursorPos);
                write_stuff(board, font, rose, crates, targets, crates_on_targets, number_of_moves, number_of_lvls_in_set, number_of_current_lvl_selected+1, valid_lvl, true, lvl_won, PlayerPos);
            }

            else if(choosing_lvl)
            {
                lvl_select(font, number_of_current_lvl_set_selected, &number_of_lvl_sets, current_lvl_set_selected_name);
                //fprintf(stderr, "%s\n", current_lvl_selected);
            }

            else if(playing_lvl)
            {
                draw_lvl(board, hex, false, CursorPos);
                write_stuff(board, font, rose, crates, targets, crates_on_targets, number_of_moves, number_of_lvls_in_set, number_of_current_lvl_selected+1, valid_lvl, false, lvl_won, PlayerPos);
            }

            else if(in_options)
            {
                options_function(move_horizontal, &font, &big_font, &hex, options_position);
            }

            else if(in_credits)
            {
                draw_credits(font, big_font);
            }

            //fprintf(stderr, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7], key[8], key[9], key[10], key[11], key[12], key[13], key[14]);           //EPIC MATRIX


            al_flip_display();
        }
    }

    deinit(&display, &event_queue, &timer);
    font_deinit(&font, &big_font);
    bitmap_deinit(&hex);
    rose_deinit(&rose);

    return 0;
}
