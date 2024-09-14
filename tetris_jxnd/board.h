#ifndef __BOARD_H_
#define __BOARD_H_
#include <SDL.h>
#include <stdbool.h>
#include "shape.h"
#define BOARD_H 16
#define BOARD_W 10
#define BLOCKSIZE   30
typedef struct {
    SDL_Color color;
    bool active;
}Block;

typedef struct {
    Block matrix[BOARD_H][BOARD_W];
    int score;
    int level;
    int clearlines;
}Gameboard;

void board_init();
void board_draw();
bool board_check_collision(int newx, int newy, Shape *shape);
void board_place_shape(Shape *shape);
void board_clear_full_lines();
int board_getlevel();

#endif