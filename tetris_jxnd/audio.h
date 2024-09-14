#ifndef __AUDIO_H__
#define __AUDIO_H__
#include <SDL_mixer.h>
void audio_init();
void audio_free();
void play_sound_rotate();
void play_sound_move();
void play_sound_down();
void play_sound_clearline();
void play_sound_bgm();
#endif