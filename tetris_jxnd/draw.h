#ifndef _DRAW_H
#define _DRAW_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

//in devcpp setup:
//-lmingw32
//-lsdl2main
//-lsdl2
//-limm32
//-lsdl2_ttf
#if 0 //在Windows中把这个0变成1
#include <SDL_syswm.h>
#include <windows.h>
#include <imm.h>
#endif

#define WINDOW_W 480
#define WINDOW_H 640

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font_30;//30
	TTF_Font* font_12;//12
	SDL_Texture* background;

}Renderer;

bool draw_init();
void draw_free();
void draw_rect(const SDL_Rect *rect, const SDL_Color *color);
void draw_update();
void draw_string(int x, int y, const char *str, SDL_Color *color, int s);

#endif
