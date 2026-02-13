#include "../includes/life.h"

int bound_neighbours(const t_life *life, const int x, const int y) {
    int n = 0;
    for (int yy = -1; yy <= 1; yy++) {
        for (int xx = -1; xx <= 1; xx++) {
            if ((xx || yy)
                && yy + y < life->ch
                && xx + x < life->cw
                && yy + y >= 0
                && xx + x >= 0)
                n += life->curGen[yy + y][xx + x];
        }
    }
    return n;
}

int unbound_neighbours(const t_life *life, const int x, const int y) {
    int **t = life->curGen;
    return t[y - 1][x - 1] + t[y - 1][x] + t[y - 1][x + 1]
            +t[y][x - 1]  + t[y][x + 1]
            +t[y + 1][x - 1] + t[y + 1][x] + t[y + 1][x + 1];
}

int count_neighbours(const t_life *life, const int x, const int y) {
    if (x - 1 >= 0 && x + 1 < life->cw
        && y - 1 >= 0 && y + 1 < life->ch) {
        return unbound_neighbours(life, x, y);
    }
    return bound_neighbours(life, x, y);
}

void game_of_life(t_life *life) {
    int neigh;
    int prev;
    life->generations++;
    if (life->curGen == nullptr)
        return;
    for (int y = 0; y < life->ch; y++) {
        for (int x = 0; x < life->cw; x++) {
            neigh = count_neighbours(life, x, y);
            prev = life->curGen[y][x];
            life->nextGen[y][x] = prev ? (neigh == 2 || neigh == 3) : (neigh == 3);
        }
    }
    for (int y = 0; y < life->ch; y++) {
        for (int x = 0; x < life->cw; x++) {
            life->curGen[y][x] = life->nextGen[y][x];
        }
    }
}
