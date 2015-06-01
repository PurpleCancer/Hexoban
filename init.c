#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "main.h"
#include "init.h"


int init(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer)
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

int deinit(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer)
{
    al_destroy_timer(*timer);
    al_destroy_display(*display);
    al_destroy_event_queue(*event_queue);

    return 0;
}

int font_init(ALLEGRO_FONT **font, ALLEGRO_FONT **big_font, char name[])
{
    char dir[100]="assets/fonts/";

    strcat(dir, name);
    strcat(dir, ".ttf");

    *font = al_load_ttf_font(dir, 20, 0);
    if(!(*font)) {
        fprintf(stderr, "failed to load the font!\n");
        al_rest(1.0);
        return -1;
    }

    *big_font = al_load_ttf_font(dir, 50, 0);
    if(!(*font)) {
        fprintf(stderr, "failed to load the font!\n");
        al_rest(1.0);
        return -1;
    }

    return 0;
}

int font_deinit(ALLEGRO_FONT **font, ALLEGRO_FONT **big_font)
{
    al_destroy_font(*font);
    al_destroy_font(*big_font);

    return 0;
}

int bitmap_init(ALLEGRO_BITMAP **hex, char name[])
{
    char dir[100]="assets/gfx/";

    strcat(dir, name);
    strcat(dir, ".png");

    *hex = al_load_bitmap(dir);
    if(!(*hex)) {
        fprintf(stderr, "failed to load hex bitmap!\n");
        al_rest(1.0);
        return -1;
    }

    return 0;
}

int bitmap_deinit(ALLEGRO_BITMAP **hex)
{
    al_destroy_bitmap(*hex);

    return 0;
}

int load_defaults(ALLEGRO_FONT **font, ALLEGRO_FONT **big_font, ALLEGRO_BITMAP **hex)
{
    FILE *ffont;
    FILE *fgfx;
    int i;
    const int length=20;

    char *fonts_names;
    char *gfx_names;

    char font_dir[35]="";
    char gfx_dir[35]="";

    int fonts_n, fonts_default;
    int gfx_n, gfx_default;

    ffont=fopen("assets/fonts/config.txt","r");
    fgfx=fopen("assets/gfx/config.txt","r");

    fscanf(ffont, "%d %d", &fonts_n, &fonts_default);
    fonts_names=(char*) calloc(fonts_n, length*sizeof(char));
    for(i=0; i<fonts_n; ++i)
    {
        fscanf(ffont, "%s", &fonts_names[length*i]);
    }
    fscanf(fgfx, "%d %d", &gfx_n, &gfx_default);
    gfx_names=(char*) calloc(gfx_n, length*sizeof(char));
    for(i=0; i<gfx_n; ++i)
    {
        fscanf(fgfx, "%s", &gfx_names[length*i]);
    }

    strcpy(font_dir,&fonts_names[length*fonts_default]);
    strcpy(gfx_dir,&gfx_names[length*gfx_default]);

    if(font_init(font, big_font, font_dir)==-1
        || bitmap_init(hex, gfx_dir)==-1)return -1;


    free(fonts_names);
    free(gfx_names);

    fclose(ffont);
    fclose(fgfx);

    return 0;
}
