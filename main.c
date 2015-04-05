#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

const int HexW=60, HexH=52;                     //dimensions of a single hex
const int OriginX=100, OriginY=100;             //origin of the board
const int BoardW=15, BoardH=10;                 //size of board in hexes
const float FPS=60;
const double min_time=0.2;                      //minimal number of second between moves
enum keys {Q, W, E, A, S, D};

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

int init(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP *hex)
//DOESN'T WORK
{
    if(!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if(!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize image addon\n");
        return -1;
    }

    /*timer = al_create_timer(1.0 / FPS);
    if(!timer) {
       fprintf(stderr, "failed to create timer!\n");
       return -1;
    }*/

    /*display = al_create_display(1280, 720);
    if(!display)
    {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    hex = al_load_bitmap("assets/gfx/hex.png");
    if(!hex)
    {
        fprintf(stderr, "failed to load hex bitmap!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_clear_to_color(al_map_rgb(255,255,255));
    al_flip_display();

    al_start_timer(timer);

    srand(time(NULL));*/

    return 0;
}

int deinit(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP *hex)
{
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(hex);

    return 0;
}

int draw(struct hex board[BoardW][BoardH+1], ALLEGRO_BITMAP *hex)       //draws the board on the display
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

int print_stuff(struct hex board[BoardW][BoardH+1], ALLEGRO_FONT *font, int crates, int targets, int crates_on_targets)
//prints the position of the player and victory conditions
{
    char PlX[3], PlY[3];
    char PlayerX[20]="Player X: ", PlayerY[20]="Player Y: ";
    char cr[3], tg[3], ctg[3];
    char Crates[20]="Crates: ", Targets[20]="Targets: ", CratesOn[20]="Crates on targets: ";

    sprintf(PlX, "%d", PlayerPos.x);
    sprintf(PlY, "%d", PlayerPos.y);
    strcat(PlayerX, PlX);
    strcat(PlayerY, PlY);

    sprintf(cr, "%d", crates);
    sprintf(tg, "%d", targets);
    sprintf(ctg, "%d", crates_on_targets);

    strcat(Crates, cr);
    strcat(Targets, tg);
    strcat(CratesOn, ctg);

    al_draw_text(font, al_map_rgb(0,0,0), 900, 400, 0, PlayerX);
    al_draw_text(font, al_map_rgb(0,0,0), 900, 430, 0, PlayerY);
    al_draw_text(font, al_map_rgb(0,0,0), 900, 460, 0, Crates);
    al_draw_text(font, al_map_rgb(0,0,0), 900, 490, 0, tg);            //<--- japierdole czemu nie dzia³asz
    al_draw_text(font, al_map_rgb(0,0,0), 900, 520, 0, CratesOn);

    return 0;
}

int board_reset(struct hex board[BoardW][BoardH+1])
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

    return 0;
}

int board_load(struct hex board[BoardW][BoardH+1], char name[],int *crates, int *targets)
{
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
                case 't':
                case 'T':
                    (*targets)++;
                    board[j][i].tl=1;
                    board[j][i].PLAYER=false;
                    board[j][i].CRATE=false;
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

int move(char key, struct hex board[BoardW][BoardH+1], int *crates_on_targets)
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
    bool redraw = true;
    bool doexit = false;
    PlayerPos.x=-1;                                                 //initial (null) position of the player
    PlayerPos.y=-1;
    bool key[6] = {false, false, false, false, false, false};       //keeps track of what keys are pressed
    double last_move_time=0;                                        //keeps track of when the last move occurred
    int crates=0, targets=0, crates_on_targets=0;                   //keeps track of completing a level


    struct hex board[BoardW][BoardH+1];

    //replace whole init with a function
    //init(display, event_queue, timer, hex);
    //DOESN'T WORK

    if(!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if(!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize image addon\n");
        return -1;
    }

    al_init_font_addon();
    al_init_ttf_addon();

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
       fprintf(stderr, "failed to create timer!\n");
       return -1;
    }

    display = al_create_display(1280, 720);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    hex = al_load_bitmap("assets/gfx/hex.png");
    if(!hex) {
        fprintf(stderr, "failed to load hex bitmap!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        return -1;
    }

    font = al_load_ttf_font("assets/fonts/arial.ttf", 20, 0);
    if(!font) {
        fprintf(stderr, "failed to load hex bitmap!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        al_destroy_bitmap(hex);
        return -1;
    }


    al_install_keyboard();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(255,255,255));
    al_flip_display();

    srand(time(NULL));

    al_start_timer(timer);

    //about up to here
    //bullshit


    board_load(board, "assets/lvls/1.txt", &crates, &targets);


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
            if(key[Q] && al_get_time()-last_move_time>min_time)if(move('q', board, &crates_on_targets)==1)last_move_time=al_get_time();
            if(key[W] && al_get_time()-last_move_time>min_time)if(move('w', board, &crates_on_targets)==1)last_move_time=al_get_time();
            if(key[E] && al_get_time()-last_move_time>min_time)if(move('e', board, &crates_on_targets)==1)last_move_time=al_get_time();
            if(key[A] && al_get_time()-last_move_time>min_time)if(move('a', board, &crates_on_targets)==1)last_move_time=al_get_time();
            if(key[S] && al_get_time()-last_move_time>min_time)if(move('s', board, &crates_on_targets)==1)last_move_time=al_get_time();
            if(key[D] && al_get_time()-last_move_time>min_time)if(move('d', board, &crates_on_targets)==1)last_move_time=al_get_time();

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

            case ALLEGRO_KEY_ESCAPE:
                doexit=true;
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
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw=false;
            al_clear_to_color(al_map_rgb(255,255,255));

            draw(board,hex);
            print_stuff(board, font, crates, targets, crates_on_targets);

            //fprintf(stderr, "%d %d %d %d %d %d\n", key[0], key[1], key[2], key[3], key[4], key[5]);           //EPIC MATRIX


            al_flip_display();
        }

    }


    //deinit(display, event_queue, timer, hex);
    //deinit: replace with a function as well

    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(hex);


    return 0;
}
