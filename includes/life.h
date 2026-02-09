#ifndef LIFE_H
# define LIFE_H

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "time.h"

#define FORMAT "Incorrect number of arguments\n"
#define USAGE "Usage: ./life [width] [height]"

typedef struct s_life {
    SDL_Window  *window;
    bool        running;
    int         cellsize;
    int         width;
    int         cw;
    int         height;
    int         ch;
    long        generations;
    int         **curGen;
    int         **nextGen;
}   t_life;

void game_of_life(t_life *life);
bool draw_life(t_life *life);
void generate_map(t_life *life);

#endif
