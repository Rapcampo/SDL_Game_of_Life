#ifndef LIFE_H
# define LIFE_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

/*============================= Ansi colours =================================*/

# define RST "\e[0m"
# define ULI "\e[4m"
# define BLN "\e[5m"
# define BLK "\e[1;30m"
# define RED "\e[1;31m"
# define GRN "\e[1;32m"
# define YLW "\e[1;33m"
# define BLU "\e[1;34m"
# define PRP "\e[1;35m"
# define CYN "\e[1;36m"
# define WHT "\e[1;37m"
# define CLR "\e[0;39m"

#define FORMAT "\e[5;1;31mIncorrect number of arguments\e[0m\n"
#define USAGE "\e[1;33mUsage:\e[0m ./life [width] [height] <shader> <hexcolor>\n"
#define SHADERS "\n\t\e[1;32mShader options:\e[0;39m\n \
[0] - no shader\n \
[1] - \e[1;31mR\e[1;32ma\e[1;33mi\e[1;34mn\e[1;35mb\e[1;36mo\e[1;31mw\e[1;37m(default)\e[0;39m\n \
[2] - \e[1;35mPurple\e[0;39m\n \
[3] - \e[1;31mOrange\e[0m\n \
[4] - \e[1;36mCustom hex: accepted formats [0xFFFFFF][FFFFFF][ffffff]\e[0m\n\n"

typedef struct s_scale {
    float     scale;
    float     off_x;
    float     off_y;
} t_scale;

typedef struct s_color {
    float     r;
    float     g;
    float     b;
} t_color;


typedef struct s_display {
    int         display_num;
    SDL_Rect    DisplayBounds;
    SDL_DisplayID *displays;
}   t_display;

typedef struct s_life {
    SDL_Window  *window;
    SDL_Renderer *renderer;
    bool        running;
    int         cellsize;
    int         width, height;
    int         requestedWidth, requestedHeight;
    int         cols, row;
    long		generations;
    int         **curGen, **nextGen;
    int         shader;
    t_color     color;
    t_scale     view;
    t_display   disp;
}   t_life;

void game_of_life(t_life *life);
void draw_life(t_life *life);
void generate_map(t_life *life);
void hexToFloatColor(t_color *color, ulong hexnbr);

#endif
