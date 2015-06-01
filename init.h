#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

const static int ScreenW=1280, ScreenH=720;            //dimensions of the screen
const static int HexW=60, HexH=52;                     //dimensions of a single hex
const static int OriginX=100, OriginY=100;             //origin of the board
const static int BoardW=15, BoardH=10;                 //size of board in hexes
const static float FPS=60;
const static double min_time=0.15;                     //minimal number of second between moves
enum keys {Q, W, E, A, S, D, UP, DOWN, RIGHT, LEFT, PGUP, PGDOWN, ENTER, ESC, MINUS, O, P, X, C, T, R};
//creating structure for the board
enum tile {wall, floor};
struct hex
{
    enum tile tl;
    int var;
    bool PLAYER;
    bool CRATE;
    bool TARGET;
};

struct pos
{
    int x;
    int y;
};

int init(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer, ALLEGRO_BITMAP **hex);
int deinit(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer, ALLEGRO_BITMAP **hex);
int font_init(ALLEGRO_FONT **font, ALLEGRO_FONT **big_font);
int font_deinit(ALLEGRO_FONT **font, ALLEGRO_FONT **big_font);

#endif // INIT_H_INCLUDED
