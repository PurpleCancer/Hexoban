#include <stdio.h>
#include <allegro5/allegro.h>

const int HexW=60, HexH=52;
const int OriginX=100, OriginY=100;
const int MapW=15, MapH=10;

enum tile {floor, wall};

struct hex
{
    enum tile tl;
    bool PLAYER;
    bool CRATE;
    bool TARGET;
};


int main(int argc, char **argv)
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *hex = NULL;

    if(!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
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
        return -1;
    }

    hex = al_load_bitmap("assets/gfx/basichex.png");
    if(!hex)
    {
        fprintf(stderr, "failed to load hex bitmap!\n");
        al_destroy_display(display);
        return -1;
    }



    al_clear_to_color(al_map_rgb(255,255,255));

    int i,j;
    int X,Y,YY;
    for(i=0,Y=OriginY;i<=MapH;++i,Y+=52)
    {
        for(j=0,X=OriginX,YY=Y;j<MapW;++j,X+=45)
        {
            if(i<MapH||j%2==1)al_draw_bitmap(hex, X, YY, 0);
            if(j%2)YY+=26;
            else YY-=26;
        }
    }

    al_draw_pixel(OriginX, OriginY, al_map_rgb(255,0,0));
    //al_draw_bitmap(hex, OriginX, OriginY, 0);

    al_flip_display();

    al_rest(5.0);

    al_destroy_display(display);

    return 0;
}
