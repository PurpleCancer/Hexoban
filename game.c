#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "decl.h"
#include "game.h"

int draw_menu(ALLEGRO_FONT *font, ALLEGRO_FONT *big_font, int position)
{
    al_draw_text(big_font, al_map_rgb(0,0,0), ScreenW/2, 80, ALLEGRO_ALIGN_CENTRE, "HEXOBAN");
    al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2+30, 120, 0, "an ugly game");

    if(position==0)al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 200, ALLEGRO_ALIGN_CENTRE, "Select level");
    else al_draw_text(font, al_map_rgb(150,150,150), ScreenW/2, 200, ALLEGRO_ALIGN_CENTRE, "Select level");

    if(position==1)al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 250, ALLEGRO_ALIGN_CENTRE, "Level editor");
    else al_draw_text(font, al_map_rgb(150,150,150), ScreenW/2, 250, ALLEGRO_ALIGN_CENTRE, "Level editor");

    if(position==2)al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 300, ALLEGRO_ALIGN_CENTRE, "Options");
    else al_draw_text(font, al_map_rgb(150,150,150), ScreenW/2, 300, ALLEGRO_ALIGN_CENTRE, "Options");

    if(position==3)al_draw_text(font, al_map_rgb(0,0,0), ScreenW/2, 350, ALLEGRO_ALIGN_CENTRE, "Exit");
    else al_draw_text(font, al_map_rgb(150,150,150), ScreenW/2, 350, ALLEGRO_ALIGN_CENTRE, "Exit");
}

int options_function(int moveh, ALLEGRO_FONT **font, ALLEGRO_FONT **big_font, ALLEGRO_BITMAP **hex, int position)
{
    FILE *fgfx;
    FILE *ffont;
    int i;
    const int length=20;

    char *fonts_names;
    char *gfx_names;

    char font_dir[35]="";
    char gfx_dir[35]="";

    int fonts_n, fonts_current;
    int gfx_n, gfx_current;

    char font_to_display[50]="";
    char gfx_to_display[50]="";

    ffont=fopen("assets/fonts/config.txt","r");
    fgfx=fopen("assets/gfx/config.txt","r");

    fscanf(ffont, "%d %d", &fonts_n, &fonts_current);
    fonts_names=(char*) calloc(fonts_n, length*sizeof(char));
    for(i=0; i<fonts_n; ++i)
    {
        fscanf(ffont, "%s", &fonts_names[length*i]);
    }
    fscanf(fgfx, "%d %d", &gfx_n, &gfx_current);
    gfx_names=(char*) calloc(gfx_n, length*sizeof(char));
    for(i=0; i<gfx_n; ++i)
    {
        fscanf(fgfx, "%s", &gfx_names[length*i]);
    }


    //displaying
    al_draw_text(*font, al_map_rgb(0,0,0), ScreenW/2, 80, ALLEGRO_ALIGN_CENTRE, "Options:");

    if(position==0)
    {
        int new_fonts_current=fonts_current+moveh;
        if(new_fonts_current<0)new_fonts_current=0;
        if(new_fonts_current>=fonts_n)new_fonts_current=fonts_n-1;
        if(new_fonts_current!=fonts_current)
        {
            fonts_current=new_fonts_current;
            fclose(ffont);
            ffont=fopen("assets/fonts/config.txt", "w+");

            fprintf(ffont, "%d %d\n", fonts_n, fonts_current);
            for(i=0; i<fonts_n; ++i)
            {
                fprintf(ffont, "%s\n", &fonts_names[length*i]);
            }

            strcpy(font_dir, &fonts_names[length*fonts_current]);

            font_deinit(font, big_font);
            font_init(font, big_font, font_dir);
        }

        al_draw_text(*font, al_map_rgb(0,0,0), ScreenW/2, 150, ALLEGRO_ALIGN_CENTRE, "Choose font:");

        if(fonts_current!=0)strcat(font_to_display, "< ");
        strcat(font_to_display, &fonts_names[length*fonts_current]);
        if(fonts_current!=fonts_n-1)strcat(font_to_display, " >");

        al_draw_text(*font, al_map_rgb(0,0,0), ScreenW/2, 200, ALLEGRO_ALIGN_CENTRE, font_to_display);

    }
    else
    {
        al_draw_text(*font, al_map_rgb(150,150,150), ScreenW/2, 150, ALLEGRO_ALIGN_CENTRE, "Choose font:");

        if(fonts_current!=0)strcat(font_to_display, "< ");
        strcat(font_to_display, &fonts_names[length*fonts_current]);
        if(fonts_current!=fonts_n-1)strcat(font_to_display, " >");

        al_draw_text(*font, al_map_rgb(150,150,150), ScreenW/2, 200, ALLEGRO_ALIGN_CENTRE, font_to_display);
    }


    if(position==1)
    {
        int new_gfx_current=gfx_current+moveh;
        if(new_gfx_current<0)new_gfx_current=0;
        if(new_gfx_current>=gfx_n)new_gfx_current=gfx_n-1;
        if(new_gfx_current!=gfx_current)
        {
            gfx_current=new_gfx_current;
            fclose(fgfx);
            fgfx=fopen("assets/gfx/config.txt", "w+");

            fprintf(fgfx, "%d %d\n", gfx_n, gfx_current);
            for(i=0; i<gfx_n; ++i)
            {
                fprintf(fgfx, "%s\n", &gfx_names[length*i]);
            }

            strcpy(gfx_dir, &gfx_names[length*gfx_current]);

            bitmap_deinit(hex);
            bitmap_init(hex, gfx_dir);
        }

        al_draw_text(*font, al_map_rgb(0,0,0), ScreenW/2, 250, ALLEGRO_ALIGN_CENTRE, "Choose graphics pack:");

        if(gfx_current!=0)strcat(gfx_to_display, "< ");
        strcat(gfx_to_display, &gfx_names[length*gfx_current]);
        if(gfx_current!=gfx_n-1)strcat(gfx_to_display, " >");

        al_draw_text(*font, al_map_rgb(0,0,0), ScreenW/2, 300, ALLEGRO_ALIGN_CENTRE, gfx_to_display);

    }
    else
    {
        al_draw_text(*font, al_map_rgb(150,150,150), ScreenW/2, 250, ALLEGRO_ALIGN_CENTRE, "Choose graphics pack:");

        if(gfx_current!=0)strcat(gfx_to_display, "< ");
        strcat(gfx_to_display, &gfx_names[length*gfx_current]);
        if(gfx_current!=gfx_n-1)strcat(gfx_to_display, " >");

        al_draw_text(*font, al_map_rgb(150,150,150), ScreenW/2, 300, ALLEGRO_ALIGN_CENTRE, gfx_to_display);
    }


    free(fonts_names);
    free(gfx_names);

    fclose(ffont);
    fclose(fgfx);

    return 0;
}

int lvl_select(ALLEGRO_FONT *font, int selected, int *number_of_lvl_sets, char lvl_selected[20])
{
    FILE *fp;
    int i;
    //int lvls_in_current_set;
    const int set_length=20;
    const int max_number_of_lvl_sets=10;
    char *sets_names;

    fp=fopen("assets/lvls/config.txt","r");

    fscanf(fp, "%d", &(*number_of_lvl_sets));
    if((*number_of_lvl_sets)>max_number_of_lvl_sets)(*number_of_lvl_sets)=max_number_of_lvl_sets;

    sets_names=(char*) calloc((*number_of_lvl_sets), set_length*sizeof(char));
    for(i=0; i<(*number_of_lvl_sets); ++i)
    {
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

int read_number_of_lvls(int *lvls_in_set, char name[])
{
    FILE *fp;
    char dir[100]="assets/lvls/";

    strcat(dir, name);
    strcat(dir, "/number.txt");

    fp=fopen(dir, "r");

    fscanf(fp, "%d", lvls_in_set);

    fclose(fp);

    return 0;
}

int write_number_of_lvls(int *lvls_in_set, char name[])
{
    FILE *fp;
    char dir[50]="assets/lvls/";

    strcat(dir, name);
    strcat(dir, "/number.txt");

    fp=fopen(dir, "w+");

    fprintf(fp, "%d", *lvls_in_set);

    fclose(fp);

    return 0;
}

int editor_function(char key, struct hex board[BoardW][BoardH+1], struct pos *CursorPos)
{
    struct pos EndPos;

    if(key=='q')
    {
        if(CursorPos->x%2)
        {
            EndPos.x=CursorPos->x-1;
            EndPos.y=CursorPos->y-1;
        }
        else
        {
            EndPos.x=CursorPos->x-1;
            EndPos.y=CursorPos->y;
        }
    }
    else if(key=='w')
    {
        EndPos.x=CursorPos->x;
        EndPos.y=CursorPos->y-1;
    }
    else if(key=='e')
    {
        if(CursorPos->x%2)
        {
            EndPos.x=CursorPos->x+1;
            EndPos.y=CursorPos->y-1;
        }
        else
        {
            EndPos.x=CursorPos->x+1;
            EndPos.y=CursorPos->y;
        }
    }
    else if(key=='a')
    {
        if(CursorPos->x%2)
        {
            EndPos.x=CursorPos->x-1;
            EndPos.y=CursorPos->y;
        }
        else
        {
            EndPos.x=CursorPos->x-1;
            EndPos.y=CursorPos->y+1;
        }
    }
    else if(key=='s')
    {
        EndPos.x=CursorPos->x;
        EndPos.y=CursorPos->y+1;
    }
    else if(key=='d')
    {
        if(CursorPos->x%2)
        {
            EndPos.x=CursorPos->x+1;
            EndPos.y=CursorPos->y;
        }
        else
        {
            EndPos.x=CursorPos->x+1;
            EndPos.y=CursorPos->y+1;
        }
    }
    else if(key=='-')
    {
        board[CursorPos->x][CursorPos->y].tl=0;
        board[CursorPos->x][CursorPos->y].PLAYER=false;
        board[CursorPos->x][CursorPos->y].CRATE=false;
        board[CursorPos->x][CursorPos->y].TARGET=false;

        return 1;
    }
    else if(key=='o')
    {
        board[CursorPos->x][CursorPos->y].tl=1;
        board[CursorPos->x][CursorPos->y].PLAYER=false;
        board[CursorPos->x][CursorPos->y].CRATE=false;
        board[CursorPos->x][CursorPos->y].TARGET=false;

        return 1;
    }
    else if(key=='p')
    {
        board[CursorPos->x][CursorPos->y].tl=1;
        board[CursorPos->x][CursorPos->y].PLAYER=true;
        board[CursorPos->x][CursorPos->y].CRATE=false;
        board[CursorPos->x][CursorPos->y].TARGET=false;

        return 1;
    }
    else if(key=='x')
    {
        board[CursorPos->x][CursorPos->y].tl=1;
        board[CursorPos->x][CursorPos->y].PLAYER=false;
        board[CursorPos->x][CursorPos->y].CRATE=false;
        board[CursorPos->x][CursorPos->y].TARGET=true;

        return 1;
    }
    else if(key=='c')
    {
        board[CursorPos->x][CursorPos->y].tl=1;
        board[CursorPos->x][CursorPos->y].PLAYER=false;
        board[CursorPos->x][CursorPos->y].CRATE=true;
        board[CursorPos->x][CursorPos->y].TARGET=false;

        return 1;
    }
    else if(key=='t')
    {
        board[CursorPos->x][CursorPos->y].tl=1;
        board[CursorPos->x][CursorPos->y].PLAYER=false;
        board[CursorPos->x][CursorPos->y].CRATE=true;
        board[CursorPos->x][CursorPos->y].TARGET=true;

        return 1;
    }

    if(EndPos.x>-1 && EndPos.y>-1 && EndPos.x<BoardW && ((EndPos.x%2==0 && EndPos.y<BoardH) || (EndPos.x%2==1 && EndPos.y<=BoardH)))
       {
            (*CursorPos)=EndPos;

            return 1;
       }


    return 0;
}

int draw_lvl(struct hex board[BoardW][BoardH+1], ALLEGRO_BITMAP *hex, bool editor, struct pos CursorPos)
//draws the board on the display
{
    int i,j;
    int X,Y,YY;
    for(i=0,Y=OriginY;i<=BoardH;++i,Y+=52)
    {
        for(j=0,X=OriginX,YY=Y;j<BoardW;++j,X+=45)
        {
            if(i<BoardH||j%2==1)
            {
                if(!board[j][i].tl)al_draw_bitmap_region(hex, board[j][i].var*HexW, 0, HexW, HexH, X, YY, 0);
                else
                {
                    al_draw_bitmap_region(hex, board[j][i].var*HexW, HexH, HexW, HexH, X, YY, 0);
                    if(board[j][i].TARGET)al_draw_bitmap_region(hex, 0, 4*HexH, HexW, HexH, X, YY, 0);
                    if(board[j][i].CRATE && !board[j][i].TARGET)al_draw_bitmap_region(hex, 0, 3*HexH, HexW, HexH, X, YY, 0);
                    if(board[j][i].CRATE && board[j][i].TARGET)al_draw_bitmap_region(hex, HexW, 3*HexH, HexW, HexH, X, YY, 0);
                    if(board[j][i].PLAYER)al_draw_bitmap_region(hex, 0, 2*HexH, HexW, HexH, X, YY, 0);
                }
                if(editor && j==CursorPos.x && i==CursorPos.y)al_draw_bitmap_region(hex, 0, 5*HexH, HexW, HexH, X, YY, 0);
            }
            YY+=j%2?26:-26;
            //if(j%2)YY+=26;
            //else YY-=26;
        }
    }

    return 0;
}

int write_stuff(struct hex board[BoardW][BoardH+1], ALLEGRO_FONT *font, int crates, int targets, int crates_on_targets, int moves, int levels, int selected, bool valid, bool editor, bool lvl_won, struct pos PlayerPos)
{
    al_draw_textf(font, al_map_rgb(0,0,0), 900, 100, 0, "Level %d of %d", selected, levels);

    if(editor)
    {
        al_draw_text(font, al_map_rgb(0,0,0), 900, 200, 0, "Use Q, W, E, A, S, D to move the cursor.");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 230, 0, "Page Down and Page Up to move");
        al_draw_text(font, al_map_rgb(0,0,0), 1000, 260, 0, "between levels.");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 290, 0, "- : puts a wall");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 320, 0, "O : puts an empty floor tile");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 350, 0, "P : puts the player");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 380, 0, "C : puts a crate");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 410, 0, "X : puts an empty target tile");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 440, 0, "T : puts a target tile with a crate on it");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 470, 0, "R : resets the level");
    }
    else
    {
        al_draw_text(font, al_map_rgb(0,0,0), 900, 200, 0, "Use Q, W, E, A, S, D to move the player.");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 230, 0, "Page Down and Page Up to move");
        al_draw_text(font, al_map_rgb(0,0,0), 1000, 260, 0, "between levels.");
        al_draw_text(font, al_map_rgb(0,0,0), 900, 290, 0, "Press R to reset the level");
    }
    if(!editor && valid)
    {
        al_draw_textf(font, al_map_rgb(0,0,0), 900, 400, 0, "Player X: %d", PlayerPos.x);
        al_draw_textf(font, al_map_rgb(0,0,0), 900, 430, 0, "Player Y: %d", PlayerPos.y);
        al_draw_textf(font, al_map_rgb(0,0,0), 900, 460, 0, "Crates: %d of %d", crates_on_targets, targets);;
        al_draw_textf(font, al_map_rgb(0,0,0), 900, 490, 0, "Moves: %d", moves);
        if(lvl_won)al_draw_text(font, al_map_rgb(0,0,0), 900, 550, 0, "YOU WON!!!!11");
    }
    else if(!editor && !valid) al_draw_text(font, al_map_rgb(0,0,0), 900, 150, 0, "Error reading the level!");

    return 0;
}

int board_reset(struct hex board[BoardW][BoardH+1], int *crates, int *targets, int *crates_on_targets, int *number_of_moves, bool *lvl_won, struct pos *PlayerPos)
{
    int i,j;
    /*for(i=0;i<BoardH;++i)
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
    }*/

    for(i=0;i<BoardH;++i)
    {
        for(j=0;j<BoardW;++j)
        {
            board[j][i].tl=0;
            board[j][i].var=rand()%4;
            board[j][i].PLAYER=false;
            board[j][i].CRATE=false;
            board[j][i].TARGET=false;
        }
    }
    for(j=1;j<BoardW;j+=2)
    {
        board[j][BoardH].tl=0;
        board[j][i].var=rand()%4;
        board[j][i].PLAYER=false;
        board[j][i].CRATE=false;
        board[j][i].TARGET=false;
    }

    PlayerPos->x=-1;
    PlayerPos->y=-1;
    *crates=0;
    *targets=0;
    *crates_on_targets=0;
    *number_of_moves=0;
    *lvl_won=false;

    return 0;
}

int board_load(struct hex board[BoardW][BoardH+1], char lvl_name[], int lvl_number, int *crates, int *targets, int *crates_on_targets, int *moves, bool *valid, bool editor, bool *lvl_won, struct pos *PlayerPos)
{
    *valid=true;

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
                    board[j][i].var=rand()%4;
                    board[j][i].PLAYER=false;
                    board[j][i].CRATE=false;
                    board[j][i].TARGET=false;
                    break;
                case 'o':
                case 'O':
                case '0':
                    board[j][i].tl=1;
                    board[j][i].var=rand()%4;
                    board[j][i].PLAYER=false;
                    board[j][i].CRATE=false;
                    board[j][i].TARGET=false;
                    break;
                case 'p':
                case 'P':
                    if(PlayerPos->x!=-1){(*valid=false);if(!editor)fprintf(stderr,"player placed two times!\n");}
                    PlayerPos->x=j;
                    PlayerPos->y=i;
                    board[j][i].tl=1;
                    board[j][i].var=rand()%4;
                    board[j][i].PLAYER=true;
                    board[j][i].CRATE=false;
                    board[j][i].TARGET=false;
                    break;
                case 'c':
                case 'C':
                    (*crates)++;
                    board[j][i].tl=1;
                    board[j][i].var=rand()%4;
                    board[j][i].PLAYER=false;
                    board[j][i].CRATE=true;
                    board[j][i].TARGET=false;
                    break;
                case 'x':
                case 'X':
                    (*targets)++;
                    board[j][i].tl=1;
                    board[j][i].var=rand()%4;
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
                    board[j][i].var=rand()%4;
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
    if(*crates!=*targets)
    {
        if(!editor)fprintf(stderr, "wrong crates/targets ratio!\n");
        *valid=false;
    }
    if(PlayerPos->x==-1)
    {
        if(!editor)fprintf(stderr, "no player placed!\n");
        *valid=false;
    }

    if(!((*valid) || editor))board_reset(board, &crates, &targets, &crates_on_targets, &moves, &lvl_won, PlayerPos);

    fclose(fp);

    return 0;
}

int board_unload(struct hex board[BoardW][BoardH+1], char lvl_name[], int lvl_number)
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

    fp=fopen(name, "w+");

    for(i=0;i<=BoardH;++i)
    {
        for(j=0;j<BoardW;++j)
        {
            if(i<BoardH||j%2==1)
            {
                if(board[j][i].tl==0)fprintf(fp, "-");
                else if(!board[j][i].PLAYER && !board[j][i].CRATE && !board[j][i].TARGET)fprintf(fp, "o");
                else if(board[j][i].PLAYER)fprintf(fp, "P");
                else if(board[j][i].CRATE && !board[j][i].TARGET)fprintf(fp, "C");
                else if(board[j][i].CRATE && board[j][i].TARGET)fprintf(fp, "T");
                else if(!board[j][i].CRATE && board[j][i].TARGET)fprintf(fp, "X");
            }
            else fprintf(fp, "-");
        }
        fprintf(fp, "\n");
    }

    fclose(fp);

    return 0;
}

int move(char key, struct hex board[BoardW][BoardH+1], int *crates_on_targets, int *moves, int targets, bool *lvl_won, struct pos *PlayerPos)
{
    struct pos EndPos;

    if(key=='q')
    {
        if(PlayerPos->x%2)
        {
            EndPos.x=PlayerPos->x-1;
            EndPos.y=PlayerPos->y-1;
        }
        else
        {
            EndPos.x=PlayerPos->x-1;
            EndPos.y=PlayerPos->y;
        }
    }
    else if(key=='w')
    {
        EndPos.x=PlayerPos->x;
        EndPos.y=PlayerPos->y-1;
    }
    else if(key=='e')
    {
        if(PlayerPos->x%2)
        {
            EndPos.x=PlayerPos->x+1;
            EndPos.y=PlayerPos->y-1;
        }
        else
        {
            EndPos.x=PlayerPos->x+1;
            EndPos.y=PlayerPos->y;
        }
    }
    else if(key=='a')
    {
        if(PlayerPos->x%2)
        {
            EndPos.x=PlayerPos->x-1;
            EndPos.y=PlayerPos->y;
        }
        else
        {
            EndPos.x=PlayerPos->x-1;
            EndPos.y=PlayerPos->y+1;
        }
    }
    else if(key=='s')
    {
        EndPos.x=PlayerPos->x;
        EndPos.y=PlayerPos->y+1;
    }
    else if(key=='d')
    {
        if(PlayerPos->x%2)
        {
            EndPos.x=PlayerPos->x+1;
            EndPos.y=PlayerPos->y;
        }
        else
        {
            EndPos.x=PlayerPos->x+1;
            EndPos.y=PlayerPos->y+1;
        }
    }

    if(EndPos.x>-1 && EndPos.y>-1 && EndPos.x<BoardW && ((EndPos.x%2==0 && EndPos.y<BoardH) || (EndPos.x%2==1 && EndPos.y<=BoardH)) &&
        board[EndPos.x][EndPos.y].tl && !board[EndPos.x][EndPos.y].CRATE)
       {
            board[PlayerPos->x][PlayerPos->y].PLAYER=false;
            board[EndPos.x][EndPos.y].PLAYER=true;
            (*PlayerPos)=EndPos;

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

                board[PlayerPos->x][PlayerPos->y].PLAYER=false;           //player movement
                board[EndPos.x][EndPos.y].PLAYER=true;
                (*PlayerPos)=EndPos;

                if(*crates_on_targets>=targets)*lvl_won=true;

                (*moves)++;
                return 1;

            }
        }

    return 0;
}
