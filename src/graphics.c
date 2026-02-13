#include "../includes/life.h"

void    allocate_maps(t_life *life) {
    life->curGen = calloc(life->ch * life->cw, sizeof(int*) + 1);
    for (int i = 0; i < life->cw; i++) {
        life->curGen[i] = calloc(life->cw * life->ch ,sizeof(int) + 1);
    }
    life->nextGen = calloc(life->ch * life->cw, sizeof(int*) + 1);
    for (int i = 0; i < life->cw; i++) {
        life->nextGen[i] = calloc(life->cw * life->ch ,sizeof(int) + 1);
    }
}

void    generate_map(t_life *life) {
    //int bigger = life->width > life->height ? life->width : life->height;
    int targetGrid = 120 * (life->width * 0.2);
    int cellpx = life->width / targetGrid;
    if (cellpx < 4)
        cellpx = 4;
    if (cellpx > 20)
        cellpx = 20;
    life->cellsize = cellpx;
    life->cw = life->width / life->cellsize;
    life->ch = life->height / life->cellsize;
    int grid_w = life->cw * life->cellsize;
    int grid_h = life->ch * life->cellsize;
    int offset_x = (life->width - grid_w) >> 1, offset_y = (life->height - grid_h) >> 1;
    life->view.off_x = offset_x;
    life->view.off_y = offset_y;

    allocate_maps(life);

    srand(time(NULL));
    double xfactor;
    const double coeficients[3] = {0.13, 0.18, 0.25};
    double seed = coeficients[(rand() % 3)];
    for (int y = 0; y < life->ch; y++) {
        for (int x = 0; x < life->cw; x++) {
            xfactor = (double)rand() / (double)RAND_MAX;
            life->curGen[y][x] = (xfactor < seed) ? 1 : 0;
        }
    }
}

void    draw_life(t_life *life) {
    //color rotation factor
    double time = (double)SDL_GetTicks() / 400;
    const float r = (float)(0.5 + 0.5 * SDL_sin(time));
    const float g = (float)(0.5 + 0.5 * SDL_sin(time + SDL_PI_D * 2 / 3));
    const float b = (float)(0.5 + 0.5 * SDL_sin(time + SDL_PI_D * 4 / 3));

    SDL_SetRenderDrawColor(life->renderer, 0, 0, 0, 255);
    SDL_RenderClear(life->renderer);

    SDL_SetRenderDrawColorFloat(life->renderer, r, g, b, SDL_ALPHA_OPAQUE_FLOAT);
    for (int y = 0; y < life->ch; y++) {
        for (int x = 0; x < life->cw; x++) {
            if (!life->curGen[y][x])
                continue;
            SDL_FRect rect = {life->view.off_x + x * life->cellsize, life->view.off_y + y * life->cellsize,
                (float)life->cellsize, (float)life->cellsize};
            SDL_RenderFillRect(life->renderer, &rect);
        }
    }
    SDL_RenderPresent(life->renderer);
}