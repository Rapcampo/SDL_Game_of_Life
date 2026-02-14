#include "../includes/life.h"

void    allocate_maps(t_life *life) {
    life->curGen = calloc(life->ch * 1, sizeof(*life->curGen) + 1);
    life->nextGen = calloc(life->ch * 1, sizeof(*life->nextGen) + 1);
    for (int i = 0; i < life->ch; i++) {
        life->curGen[i] = calloc(life->cw * 1 ,sizeof(**life->curGen) + 1);
        life->nextGen[i] = calloc(life->cw * 1 ,sizeof(**life->nextGen) + 1);
    }
}

void    generate_map(t_life *life) {
    int bigger = life->req_width > life->req_height ? life->req_width : life->req_height;
    int targetGrid = 120 * (bigger * 0.2);
    int cellpx = bigger / targetGrid;
    if (cellpx < 4)
        cellpx = 4;
    if (cellpx > 20)
        cellpx = 20;
    life->cellsize = cellpx;
    life->cw = life->req_width / life->cellsize;
    life->ch = life->req_height / life->cellsize;
    int grid_w = life->cw * life->cellsize;
    int grid_h = life->ch * life->cellsize;
    int offset_x = (life->req_width - grid_w) >> 1, offset_y = (life->req_height - grid_h) >> 1;
    life->view.off_x = offset_x;
    life->view.off_y = offset_y;

    allocate_maps(life);

    srand(time(NULL));
    double xfactor;
    const double coeficients[3] = {0.08, 0.13, 0.15};
    double seed = coeficients[(rand() % 3)];
    for (int y = 0; y < life->ch; y++) {
        for (int x = 0; x < life->cw; x++) {
            xfactor = (double)rand() / (double)RAND_MAX;
            life->curGen[y][x] = (xfactor < seed) ? 1 : 0;
        }
    }
}

void    rainbow_shader(float *r, float *g, float *b) {
    //color rotation factor
    double time = (double)SDL_GetTicks() / 400;
    *r = (float)(0.5 + 0.5 * SDL_sin(time));
    *g = (float)(0.5 + 0.5 * SDL_sin(time + SDL_PI_D * 2 / 3));
    *b = (float)(0.5 + 0.5 * SDL_sin(time + SDL_PI_D * 4 / 3));
}

void   purple_shader(float *r, float *g, float *b) {
    double time = (double)SDL_GetTicks() / 400;
    float intensity = 0.9f + 0.7f * (0.5f + 0.5f * SDL_sin(time));
    *r = 0.702f * intensity;
    *g = 0.02f * intensity;
    *b = 0.969 * intensity;
}

void   orange_shader(float *r, float *g, float *b) {
    double time = (double)SDL_GetTicks() / 400;
    float intensity = 0.8f + 0.5f * (0.5f + 0.5f * SDL_sin(time));
    *r = 0.89f * intensity;
    *g = 0.314f * intensity;
    *b = 0.075f * intensity;
}

void    select_shader(float *r, float *g, float *b, int shader) {
    switch (shader) {
        case 1:
            rainbow_shader(r, g, b);
            break;
        case 2:
            purple_shader(r, g, b);
            break;
        case 3:
            orange_shader(r, g, b);
            break;
        default:
            *r = 255.0f, *g = 255.0f, *b = 255.0f;
            break;
    }
}

void    draw_life(t_life *life) {
    SDL_SetRenderDrawColor(life->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(life->renderer);

    float r,g,b;
    select_shader(&r, &g, &b, life->shader);
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