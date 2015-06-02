#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

int init(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer);
int deinit(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **event_queue, ALLEGRO_TIMER **timer);
int font_init(ALLEGRO_FONT **font, ALLEGRO_FONT **big_font, char name[]);
int font_deinit(ALLEGRO_FONT **font, ALLEGRO_FONT **big_font);
int bitmap_init(ALLEGRO_BITMAP **hex, char name[]);
int bitmap_deinit(ALLEGRO_BITMAP **hex);
int rose_init(ALLEGRO_BITMAP **rose);
int rose_deinit(ALLEGRO_BITMAP **rose);
int load_defaults(ALLEGRO_FONT **font, ALLEGRO_FONT **big_font, ALLEGRO_BITMAP **hex, ALLEGRO_BITMAP **rose);

#endif // INIT_H_INCLUDED
