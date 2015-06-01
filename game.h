#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

int draw_menu(ALLEGRO_FONT *font, ALLEGRO_FONT *big_font, int position);
int options_function(int moveh, ALLEGRO_FONT **font, ALLEGRO_FONT **big_font, ALLEGRO_BITMAP **hex, int position);
int lvl_select(ALLEGRO_FONT *font, int selected, int *number_of_lvl_sets, char lvl_selected[20]);
int read_number_of_lvls(int *lvls_in_set, char name[]);
int write_number_of_lvls(int *lvls_in_set, char name[]);
int editor_function(char key, struct hex board[BoardW][BoardH+1], struct pos *CursorPos);
int draw_lvl(struct hex board[BoardW][BoardH+1], ALLEGRO_BITMAP *hex, bool editor, struct pos CursorPos);
int write_stuff(struct hex board[BoardW][BoardH+1], ALLEGRO_FONT *font, int crates, int targets, int crates_on_targets, int moves, int levels, int selected, bool valid, bool editor, bool lvl_won, struct pos PlayerPos);
int board_reset(struct hex board[BoardW][BoardH+1], int *crates, int *targets, int *crates_on_targets, int *number_of_moves, bool *lvl_won, struct pos *PlayerPos);
int board_load(struct hex board[BoardW][BoardH+1], char lvl_name[], int lvl_number, int *crates, int *targets, int *crates_on_targets, int *moves, bool *valid, bool editor, bool *lvl_won, struct pos *PlayerPos);
int board_unload(struct hex board[BoardW][BoardH+1], char lvl_name[], int lvl_number);
int move(char key, struct hex board[BoardW][BoardH+1], int *crates_on_targets, int *moves, int targets, bool *lvl_won, struct pos *PlayerPos);

#endif // GAME_H_INCLUDED
