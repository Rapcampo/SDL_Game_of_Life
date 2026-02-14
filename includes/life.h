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

#define FORMAT "Incorrect number of arguments\n"
#define USAGE "Usage: ./life [width] [height] <shader>\n"
#define SHADERS "\nShader options:\n[0] - no shader\n[1] - Rainbow(default)\n[2] - Purple\n[3] - Orange\n\n"

typedef struct s_scale {
    float     scale;
    float     off_x;
    float     off_y;
} t_scale;

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
    int         req_width, req_height;
    int         cw, ch;
    long        generations;
    int         **curGen, **nextGen;
    int         shader;
    t_scale     view;
    t_display   disp;
}   t_life;

void game_of_life(t_life *life);
void draw_life(t_life *life);
void generate_map(t_life *life);

#endif
