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
    int targetGrid = 160 * (life->width * 0.1);
    int cellpx = life->width / targetGrid;
    if (cellpx < 4)
        cellpx = 4;
    if (cellpx > 20)
        cellpx = 20;
    life->cellsize = cellpx;
    life->cw = life->width / life->cellsize;
    life->ch = life->height / life->cellsize;

    allocate_maps(life);

    double xfactor;
    for (int y = 0; y < life->cw; y++) {
        for (int x = 0; x < life->ch; x++) {
            xfactor = (double)rand() / (double)RAND_MAX;
            life->curGen[y][x] = (xfactor < 0.25) ? 1 : 0;
        }
    }
}