#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>

const int HexW=60, HexH=52;                     //dimensions of a single hex
const int OriginX=100, OriginY=100;             //origin of the board
const int BoardW=15, BoardH=10;                 //size of board in hexes
const float FPS=60;

//creating structure for the board
enum tile {wall, floor};
struct hex
{
    enum tile tl;
    bool PLAYER;
    bool CRATE;
    bool TARGET;
};

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
                else al_draw_bitmap_region(hex, 0, HexH, HexW, HexH, X, YY, 0);
            }
            if(j%2)YY+=26;
            else YY-=26;
        }
    }

    return 0;
}


int main(int argc, char **argv)
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    bool redraw = true;
    ALLEGRO_BITMAP *hex = NULL;

    struct hex board[BoardW][BoardH+1];

    //replace whole init with a function

    if(!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
       fprintf(stderr, "failed to create timer!\n");
       return -1;
    }

    if(!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize image addon\n");
        return -1;
   }

    display = al_create_display(1280, 720);
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

    srand(time(NULL));

    //about up to here


    //board initialization: replace with a function (after adding file handling)

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


    //main loop
    while(1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw=true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw=false;
            al_clear_to_color(al_map_rgb(255,255,255));

            draw(board,hex);

            al_flip_display();
        }

    }




    //al_draw_pixel(OriginX, OriginY, al_map_rgb(255,0,0));


    //deinit: replace with a function as well
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
