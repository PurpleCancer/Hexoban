#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

const int ScreenW=1280, ScreenH=720;            //dimensions of the screen
const int HexW=60, HexH=52;                     //dimensions of a single hex
const int OriginX=100, OriginY=100;             //origin of the board
const int BoardW=15, BoardH=10;                 //size of board in hexes
const float FPS=60;
const double min_time=0.2;                      //minimal number of second between moves
enum keys {Q, W, E, A, S, D, UP, DOWN, RIGHT, LEFT, PGUP, PGDOWN, ENTER, ESC};

//creating structure for the board
enum tile {wall, floor};
struct hex
{
    enum tile tl;
    bool PLAYER;
    bool CRATE;
    bool TARGET;
};

struct pos
{
    int x;
    int y;
}PlayerPos;

int init(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer, ALLEGRO_BITMAP **hex, ALLEGRO_FONT **font)
{
    bool no_error;

    no_error=al_init();
    if(!no_error)
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        al_rest(1.0);
        return -1;
    }

    no_error=al_init_image_addon();
    if(!no_error) {
        fprintf(stderr, "failed to initialize image addon!\n");
        al_rest(1.0);
        return -1;
    }

    al_init_font_addon();

    no_error=al_init_ttf_addon();
    if(!no_error) {
        fprintf(stderr, "failed to initialize ttf addon!\n");
        al_rest(1.0);
        return -1;
    }

    *timer = al_create_timer(1.0 / FPS);
    if(!(*timer)) {
       fprintf(stderr, "failed to create timer!\n");
        al_rest(1.0);
       return -1;
    }

    *display = al_create_display(ScreenW, ScreenH);
    if(!(*display)) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(*timer);
        al_rest(1.0);
        return -1;
    }

    *event_queue = al_create_event_queue();
    if(!(*event_queue)) {
        fprintf(stderr, "failed to create event queue!\n");
        al_destroy_display(*display);
        al_destroy_timer(*timer);
        al_rest(1.0);
        return -1;
    }

    *hex = al_load_bitmap("assets/gfx/hex.png");
    if(!(*hex)) {
        fprintf(stderr, "failed to load hex bitmap!\n");
        al_destroy_display(*display);
        al_destroy_timer(*timer);
        al_destroy_event_queue(*event_queue);
        al_rest(1.0);
        return -1;
    }

    *font = al_load_ttf_font("assets/fonts/arial.ttf", 20, 0);
    if(!(*font)) {
        fprintf(stderr, "failed to load the font!\n");
        al_destroy_display(*display);
        al_destroy_timer(*timer);
        al_destroy_event_queue(*event_queue);
        al_destroy_bitmap(*hex);
        al_rest(1.0);
        return -1;
    }

    al_install_keyboard();

    al_register_event_source(*event_queue, al_get_display_event_source(*display));
    al_register_event_source(*event_queue, al_get_timer_event_source(*timer));
    al_register_event_source(*event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(255,255,255));
    al_flip_display();

    srand(time(NULL));

    al_start_timer(*timer);

    return 0;
}

int deinit(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer, ALLEGRO_BITMAP **hex, ALLEGRO_FONT **font)
{
    al_destroy_timer(*timer);
    al_destroy_display(*display);
    al_destroy_event_queue(*event_queue);
    al_destroy_bitmap(*hex);
    al_destroy_font(*font);

    return 0;
}

int draw_menu(ALLEGRO_FONT *font, int position)
{
    al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 100, ALLEGRO_ALIGN_CENTRE, "MENU");

    if(position==0)al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 200, ALLEGRO_ALIGN_CENTRE, "Select level");
    else al_draw_text(font, al_map_rgb(150,150,150), ScreenW/2, 200, ALLEGRO_ALIGN_CENTRE, "Select level");

    if(position==1)al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 250, ALLEGRO_ALIGN_CENTRE, "Level editor");
    else al_draw_text(font, al_map_rgb(150,150,150), ScreenW/2, 250, ALLEGRO_ALIGN_CENTRE, "Level editor");

    if(position==2)al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 300, ALLEGRO_ALIGN_CENTRE, "Exit");
    else al_draw_text(font, al_map_rgb(150,150,150), ScreenW/2, 300, ALLEGRO_ALIGN_CENTRE, "Exit");
}

int lvl_select(ALLEGRO_FONT *font, int selected, int *number_of_lvl_sets, char lvl_selected[20], int *lvls_in_set)
{
    FILE *fp;
    int i,j;
    int lvls_in_current_set;
    const int set_length=20;
    const int max_number_of_lvl_sets=10;
    char *sets_names;

    fp=fopen("assets/lvls/config.txt","r");

    fscanf(fp, "%d", &(*number_of_lvl_sets));
    if((*number_of_lvl_sets)>max_number_of_lvl_sets)(*number_of_lvl_sets)=max_number_of_lvl_sets;

    sets_names=(char*) calloc((*number_of_lvl_sets), set_length*sizeof(char));
    for(i=0; i<(*number_of_lvl_sets); ++i)
    {
        fscanf(fp, "%d", &lvls_in_current_set);
        if(i==selected)(*lvls_in_set)=lvls_in_current_set;
        fscanf(fp, "%s", &sets_names[set_length*i]);
    }

    al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 100, ALLEGRO_ALIGN_CENTRE, "Select a level set to play:");

    for(i=0; i<(*number_of_lvl_sets); ++i)
    {
        if(i==selected)al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 150+30*i, ALLEGRO_ALIGN_CENTRE, &sets_names[set_length*i]);
        else al_draw_text(font, al_map_rgb(150,150,150), ScreenW/2, 150+30*i, ALLEGRO_ALIGN_CENTRE, &sets_names[set_length*i]);
    }

    //fprintf(stderr, "%s\n", &sets_names[set_length*selected]);
    strcpy(lvl_selected, &sets_names[set_length*selected]);

    free(sets_names);
    fclose(fp);

    return 0;
}

int draw_lvl(struct hex board[BoardW][BoardH+1], ALLEGRO_BITMAP *hex)       //draws the board on the display
//replace numbers with constants?
{
    int i,j;
    int X,Y,YY;
    for(i=0,Y=OriginY;i<=BoardH;++i,Y+=52)
    {
        for(j=0,X=OriginX,YY=Y;j<BoardW;++j,X+=45)
        {
            if(i<BoardH||j%2==1)
            {
                if(!board[j][i].tl)al_draw_bitmap_region(hex, 0, 0, HexW, HexH, X, YY, 0);
                else
                {
                    al_draw_bitmap_region(hex, 0, HexH, HexW, HexH, X, YY, 0);
                    if(board[j][i].PLAYER)al_draw_bitmap_region(hex, 0, 2*HexH, HexW, HexH, X, YY, 0);
                    if(board[j][i].CRATE)al_draw_bitmap_region(hex, 0, 3*HexH, HexW, HexH, X, YY, 0);
                    if(board[j][i].TARGET)al_draw_bitmap_region(hex, 0, 4*HexH, HexW, HexH, X, YY, 0);
                }
            }
            YY+=j%2?26:-26;
            //if(j%2)YY+=26;
            //else YY-=26;
        }
    }

    return 0;
}

int print_stuff(struct hex board[BoardW][BoardH+1], ALLEGRO_FONT *font, int crates, int targets, int crates_on_targets, int moves, int levels, int selected)
//prints the position of the player and victory conditions
{
    bool lvl_won=false;
    if(crates_on_targets>=targets)lvl_won=true;

    char PlX[3], PlY[3];
    char PlayerX[20]="Player X: ", PlayerY[20]="Player Y: ";
    char cr[3], tg[3], ctg[3], mvs[5];
    char Crates[20]="Crates: ", Targets[20]="Targets: ", CratesOn[20]="Crates on targets: ", Moves[20]="Moves: ";
    char lvl[4] , lvls[4];
    char Level[20]="Level ";

    sprintf(PlX, "%d", PlayerPos.x);
    sprintf(PlY, "%d", PlayerPos.y);
    strcat(PlayerX, PlX);
    strcat(PlayerY, PlY);

    sprintf(cr, "%d", crates);
    sprintf(tg, "%d", targets);
    sprintf(ctg, "%d", crates_on_targets);
    sprintf(mvs, "%d", moves);

    strcat(Crates, cr);
    strcat(Targets, tg);
    strcat(CratesOn, ctg);
    strcat(Moves, mvs);

    sprintf(lvl, "%d", selected);
    sprintf(lvls, "%d", levels);
    strcat(Level, lvl);
    strcat(Level, " of ");
    strcat(Level, lvls);

    al_draw_text(font, al_map_rgb(0,0,0), 900, 100, 0, Level);
    al_draw_text(font, al_map_rgb(0,0,0), 900, 400, 0, PlayerX);
    al_draw_text(font, al_map_rgb(0,0,0), 900, 430, 0, PlayerY);
    al_draw_text(font, al_map_rgb(0,0,0), 900, 460, 0, Crates);
    al_draw_text(font, al_map_rgb(0,0,0), 900, 490, 0, tg);            //<--- japierdole czemu nie dzia³asz
    al_draw_text(font, al_map_rgb(0,0,0), 900, 520, 0, CratesOn);
    al_draw_text(font, al_map_rgb(0,0,0), 900, 550, 0, Moves);
    if(lvl_won)al_draw_text(font, al_map_rgb(0,0,0), 900, 600, 0, "YOU WON!!!!11");

    return 0;
}

int board_reset(struct hex board[BoardW][BoardH+1], int *crates, int *targets, int *crates_on_targets, int *number_of_moves)
{
    int i,j;
    for(i=0;i<BoardH;++i)
    {
        for(j=0;j<BoardW;++j)
        {
            board[j][i].tl=rand()%2;
            board[j][i].PLAYER=false;
            board[j][i].CRATE=false;
            board[j][i].TARGET=false;
        }
    }
    for(j=1;j<BoardW;j+=2)
    {
        board[j][BoardH].tl=rand()%2;
        board[j][i].PLAYER=false;
        board[j][i].CRATE=false;
        board[j][i].TARGET=false;
    }

    PlayerPos.x=-1;
    PlayerPos.y=-1;
    *crates=0;
    *targets=0;
    *crates_on_targets=0;
    *number_of_moves=0;

    return 0;
}

int board_load(struct hex board[BoardW][BoardH+1], char lvl_name[], int lvl_number, int *crates, int *targets, int *crates_on_targets)
{
    char name[100]="assets/lvls/";
    strcat(name, lvl_name);
    strcat(name, "/");
    char number[4];
    sprintf(number, "%d", lvl_number);
    strcat(name, number);
    strcat(name, ".txt");

    FILE *fp;
    int i,j;
    char s[20];

    fp=fopen(name,"r");

    for(i=0;i<=BoardH;++i)
    {
        fscanf(fp,"%s",s);
        for(j=0;j<BoardW;++j)
        {
            if(i<BoardH||j%2==1)
            {
                switch(s[j])
                {
                case '-':
                    board[j][i].tl=0;
                    board[j][i].PLAYER=false;
                    board[j][i].CRATE=false;
                    board[j][i].TARGET=false;
                    break;
                case 'o':
                case 'O':
                case '0':
                    board[j][i].tl=1;
                    board[j][i].PLAYER=false;
                    board[j][i].CRATE=false;
                    board[j][i].TARGET=false;
                    break;
                case 'p':
                case 'P':
                    if(PlayerPos.x!=-1)fprintf(stderr,"player placed two times!\n");
                    PlayerPos.x=j;
                    PlayerPos.y=i;
                    board[j][i].tl=1;
                    board[j][i].PLAYER=true;
                    board[j][i].CRATE=false;
                    board[j][i].TARGET=false;
                    break;
                case 'c':
                case 'C':
                    (*crates)++;
                    board[j][i].tl=1;
                    board[j][i].PLAYER=false;
                    board[j][i].CRATE=true;
                    board[j][i].TARGET=false;
                    break;
                case 'x':
                case 'X':
                    (*targets)++;
                    board[j][i].tl=1;
                    board[j][i].PLAYER=false;
                    board[j][i].CRATE=false;
                    board[j][i].TARGET=true;
                    break;
                case 't':
                case 'T':
                    (*targets)++;
                    (*crates)++;
                    (*crates_on_targets)++;
                    board[j][i].tl=1;
                    board[j][i].PLAYER=false;
                    board[j][i].CRATE=true;
                    board[j][i].TARGET=true;
                    break;
                default:
                    fprintf(stderr,"error reading the file\n");
                    break;
                }
            }
        }
    }
    fclose(fp);

    return 0;
}

int move(char key, struct hex board[BoardW][BoardH+1], int *crates_on_targets, int *moves)
{
    struct pos EndPos;

    if(key=='q')
    {
        if(PlayerPos.x%2)
        {
            EndPos.x=PlayerPos.x-1;
            EndPos.y=PlayerPos.y-1;
        }
        else
        {
            EndPos.x=PlayerPos.x-1;
            EndPos.y=PlayerPos.y;
        }
    }
    else if(key=='w')
    {
        EndPos.x=PlayerPos.x;
        EndPos.y=PlayerPos.y-1;
    }
    else if(key=='e')
    {
        if(PlayerPos.x%2)
        {
            EndPos.x=PlayerPos.x+1;
            EndPos.y=PlayerPos.y-1;
        }
        else
        {
            EndPos.x=PlayerPos.x+1;
            EndPos.y=PlayerPos.y;
        }
    }
    else if(key=='a')
    {
        if(PlayerPos.x%2)
        {
            EndPos.x=PlayerPos.x-1;
            EndPos.y=PlayerPos.y;
        }
        else
        {
            EndPos.x=PlayerPos.x-1;
            EndPos.y=PlayerPos.y+1;
        }
    }
    else if(key=='s')
    {
        EndPos.x=PlayerPos.x;
        EndPos.y=PlayerPos.y+1;
    }
    else if(key=='d')
    {
        if(PlayerPos.x%2)
        {
            EndPos.x=PlayerPos.x+1;
            EndPos.y=PlayerPos.y;
        }
        else
        {
            EndPos.x=PlayerPos.x+1;
            EndPos.y=PlayerPos.y+1;
        }
    }

    if(EndPos.x>-1 && EndPos.y>-1 && EndPos.x<BoardW && ((EndPos.x%2==0 && EndPos.y<BoardH) || (EndPos.x%2==1 && EndPos.y<=BoardH)) &&
        board[EndPos.x][EndPos.y].tl && !board[EndPos.x][EndPos.y].CRATE)
       {
            board[PlayerPos.x][PlayerPos.y].PLAYER=false;
            board[EndPos.x][EndPos.y].PLAYER=true;
            PlayerPos=EndPos;

            (*moves)++;
            return 1;
       }

    else if(EndPos.x>-1 && EndPos.y>-1 && EndPos.x<BoardW && ((EndPos.x%2==0 && EndPos.y<BoardH) || (EndPos.x%2==1 && EndPos.y<=BoardH)) &&
        board[EndPos.x][EndPos.y].tl && board[EndPos.x][EndPos.y].CRATE)
        {
            struct pos CratePos;

            if(key=='q')
            {
                if(EndPos.x%2)
                {
                    CratePos.x=EndPos.x-1;
                    CratePos.y=EndPos.y-1;
                }
                else
                {
                    CratePos.x=EndPos.x-1;
                    CratePos.y=EndPos.y;
                }
            }
            else if(key=='w')
            {
                CratePos.x=EndPos.x;
                CratePos.y=EndPos.y-1;
            }
            else if(key=='e')
            {
                if(EndPos.x%2)
                {
                    CratePos.x=EndPos.x+1;
                    CratePos.y=EndPos.y-1;
                }
                else
                {
                    CratePos.x=EndPos.x+1;
                    CratePos.y=EndPos.y;
                }
            }
            else if(key=='a')
            {
                if(EndPos.x%2)
                {
                    CratePos.x=EndPos.x-1;
                    CratePos.y=EndPos.y;
                }
                else
                {
                    CratePos.x=EndPos.x-1;
                    CratePos.y=EndPos.y+1;
                }
            }
            else if(key=='s')
            {
                CratePos.x=EndPos.x;
                CratePos.y=EndPos.y+1;
            }
            else if(key=='d')
            {
                if(EndPos.x%2)
                {
                    CratePos.x=EndPos.x+1;
                    CratePos.y=EndPos.y;
                }
                else
                {
                    CratePos.x=EndPos.x+1;
                    CratePos.y=EndPos.y+1;
                }
            }

            if(CratePos.x>-1 && CratePos.y>-1 && CratePos.x<BoardW && ((CratePos.x%2==0 && CratePos.y<BoardH) || (CratePos.x%2==1 && CratePos.y<=BoardH)) &&
                board[CratePos.x][CratePos.y].tl && !board[CratePos.x][CratePos.y].CRATE)
            {
                if(board[CratePos.x][CratePos.y].TARGET)(*crates_on_targets)++;
                if(board[EndPos.x][EndPos.y].TARGET)(*crates_on_targets)--;

                board[EndPos.x][EndPos.y].CRATE=false;                  //crate movement
                board[CratePos.x][CratePos.y].CRATE=true;

                board[PlayerPos.x][PlayerPos.y].PLAYER=false;           //player movement
                board[EndPos.x][EndPos.y].PLAYER=true;
                PlayerPos=EndPos;

                (*moves)++;
                return 1;

            }
        }
    return 0;
}

int main(int argc, char **argv)
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *hex = NULL;
    ALLEGRO_FONT *font = NULL;
    //game states
    bool in_menu=true, in_editor=false, choosing_lvl=false, playing_lvl=false;
    bool redraw = true;
    bool doexit = false;
    PlayerPos.x=-1;                                                 //initial (null) position of the player
    PlayerPos.y=-1;
    bool key[14] = {false, false, false, false, false, false,
                    false, false, false, false, false, false,
                    false, false};                                 //keeps track of what keys are pressed
    double last_key_time=0;                                        //keeps track of when the last move occurred
    int crates=0, targets=0, crates_on_targets=0;                   //keeps track of completing a level

    //handling level selection
    int number_of_current_lvl_set_selected=0, number_of_current_lvl_selected, number_of_lvls_in_set;
    char current_lvl_set_selected_name[20];
    int number_of_lvl_sets=0;
    int number_of_moves;

    //handling the menu
    int menu_position=0;
    const int number_of_menu_positions=3;


    struct hex board[BoardW][BoardH+1];


    if(init(&display, &event_queue, &timer, &hex, &font)==-1)return -1;

    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves);
    //board_load(board, "assets/lvls/1/1.txt", &crates, &targets, &crates_on_targets);


    /*draw(board, hex);
    al_flip_display();
    al_rest(5.0);*/

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
                    else if(menu_position==1){in_menu=false; in_editor=true;}
                    else if(menu_position==2)doexit=true;
                }
                if(key[ESC] && al_get_time()-last_key_time>min_time)doexit=true;
            }
            else if(in_editor)
            {
                //add a nice lvl editor

                if(key[ESC] && al_get_time()-last_key_time>min_time)
                {
                    last_key_time=al_get_time();
                    in_menu=true;
                    in_editor=false;
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

                    board_load(board, current_lvl_set_selected_name, number_of_current_lvl_selected, &crates, &targets, &crates_on_targets);
                }

                if(key[ESC] && al_get_time()-last_key_time>min_time)
                {
                    last_key_time=al_get_time();
                    in_menu=true;
                    choosing_lvl=false;
                }
            }
            else if(playing_lvl){
                if(key[Q] && al_get_time()-last_key_time>min_time)if(move('q', board, &crates_on_targets, &number_of_moves)==1)last_key_time=al_get_time();
                if(key[W] && al_get_time()-last_key_time>min_time)if(move('w', board, &crates_on_targets, &number_of_moves)==1)last_key_time=al_get_time();
                if(key[E] && al_get_time()-last_key_time>min_time)if(move('e', board, &crates_on_targets, &number_of_moves)==1)last_key_time=al_get_time();
                if(key[A] && al_get_time()-last_key_time>min_time)if(move('a', board, &crates_on_targets, &number_of_moves)==1)last_key_time=al_get_time();
                if(key[S] && al_get_time()-last_key_time>min_time)if(move('s', board, &crates_on_targets, &number_of_moves)==1)last_key_time=al_get_time();
                if(key[D] && al_get_time()-last_key_time>min_time)if(move('d', board, &crates_on_targets, &number_of_moves)==1)last_key_time=al_get_time();

                if(key[PGUP] && al_get_time()-last_key_time>min_time && number_of_current_lvl_selected>0){number_of_current_lvl_selected--;last_key_time=al_get_time();
                                                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves);
                                                    board_load(board, current_lvl_set_selected_name, number_of_current_lvl_selected, &crates, &targets, &crates_on_targets);}
                if(key[PGDOWN] && al_get_time()-last_key_time>min_time && number_of_current_lvl_selected<number_of_lvls_in_set-1){number_of_current_lvl_selected++;last_key_time=al_get_time();
                                                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves);
                                                    board_load(board, current_lvl_set_selected_name, number_of_current_lvl_selected, &crates, &targets, &crates_on_targets);}

                if(key[ESC] && al_get_time()-last_key_time>min_time)
                {
                    last_key_time=al_get_time();
                    choosing_lvl=true;
                    playing_lvl=false;

                    board_reset(board, &crates, &targets, &crates_on_targets, &number_of_moves);
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
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw=false;
            al_clear_to_color(al_map_rgb(255,255,255));

            if(in_menu)
            {
                draw_menu(font, menu_position);
            }

            else if(in_editor)
            {
                //add a nice lvl editor
            }

            else if(choosing_lvl)
            {
                lvl_select(font, number_of_current_lvl_set_selected, &number_of_lvl_sets, current_lvl_set_selected_name, &number_of_lvls_in_set);
                //fprintf(stderr, "%s\n", current_lvl_selected);
            }

            else if(playing_lvl)
            {
                draw_lvl(board,hex);
                print_stuff(board, font, crates, targets, crates_on_targets, number_of_moves, number_of_lvls_in_set, number_of_current_lvl_selected+1);
            }

            //fprintf(stderr, "%d %d %d %d %d %d %d %d %d %d %d %d %d\n", key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7], key[8], key[9], key[10], key[11], key[12]);           //EPIC MATRIX


            al_flip_display();
        }
    }

    deinit(&display, &event_queue, &timer, &hex, &font);

    return 0;
}
