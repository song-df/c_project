#ifndef __CONTROL_H__
#define __CONTROL_H__

typedef enum{
    STATE_UNKNOWN,
    STATE_RUNNING,
    STATE_PAUSE,
    STATE_STOP,
    STATE_GAME_OVER,
    STATE_MANUAL
}GameState;

void control_gameloop();

GameState control_getstate();
void control_setstate(GameState s);

#endif