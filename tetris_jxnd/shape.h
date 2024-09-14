#ifndef _SHAPE_H
#define _SHAPE_H
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum{
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP
}Movedir;

typedef struct {
    bool matrix[4][4];
    int x;
    int y;
    int size;
    SDL_Color color;
}Shape;


void shape_init();
void shape_draw();
void shape_move(Movedir dir);


#endif