#include "shape.h"
#include <SDL.h>
#include "control.h"
#include "draw.h"
#include "board.h"
#include "audio.h"

static GameState state;

GameState control_getstate(){
    return state;
}
void control_setstate(GameState s){
    state = s;
}

void control_gameloop(){
    // 事件循环
    SDL_Event e;
	int quit = 0;
    state = STATE_RUNNING;
    while(!quit){
	    while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT) {
                quit = 1;
	        }
			if (e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym){
					case SDLK_ESCAPE:
					case SDLK_q:
						quit = 1;
					break;
                    case SDLK_r:
                        if(state == STATE_GAME_OVER){
                            shape_init();
	                        board_init();
                            state = STATE_RUNNING;
                        }
                    break;
                    case SDLK_p://暂停
                        if(state == STATE_RUNNING){
                            state = STATE_PAUSE;
                            break;
                        }
                        if(state == STATE_PAUSE){
                            state = STATE_RUNNING;
                            break;
                        }
                    break;
                    case SDLK_a:
                        play_sound_bgm();
                        break;
                    case SDLK_m://手动和自动下落切换
                        if(state == STATE_RUNNING){
                            state = STATE_MANUAL;
                            break;
                        }
                        if(state == STATE_MANUAL){
                            state = STATE_RUNNING;
                            break;
                        }
                    break;
					case SDLK_UP:
                        if(state == STATE_RUNNING || state == STATE_MANUAL)
						    shape_move(MOVE_UP);
					break;
					case SDLK_DOWN:
                        if(state == STATE_RUNNING || state == STATE_MANUAL)
                            shape_move(MOVE_DOWN);
					break;
					case SDLK_LEFT:
                        if(state == STATE_RUNNING || state == STATE_MANUAL)
                            shape_move(MOVE_LEFT);
					break;
					case SDLK_RIGHT:
                        if(state == STATE_RUNNING || state == STATE_MANUAL)
                            shape_move(MOVE_RIGHT);
					break;
				}
			}
		}
		draw_update();
        SDL_Delay(16);//60Hz
    }
}