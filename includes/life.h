#ifndef LIFE_H
# define LIFE_H

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#define USAGE "place"
#define FORMAT "holder"

typedef struct s_life {
    SDL_Window  *window;
    char        *map;
    int         width;
    int         height;
    char        **curGen;
    char        **nextGen;
}   t_life;

bool game_of_life(t_life *life);
bool draw_life(t_life *life);

#endif
