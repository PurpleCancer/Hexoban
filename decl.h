#ifndef DECL_H_INCLUDED
#define DECL_H_INCLUDED

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


#endif // DECL_H_INCLUDED
