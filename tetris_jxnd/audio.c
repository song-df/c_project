#include "audio.h"

Mix_Music* bg_sound;
Mix_Chunk* rotate_sound;
Mix_Chunk* down_sound;
Mix_Chunk* move_sound;
Mix_Chunk* line_clear_sound;
// 加载音效
Mix_Chunk* load_sound(const char* path) {
    Mix_Chunk *sound = Mix_LoadWAV(path);
    if (sound == NULL) {
        printf("Failed to load sound effect: %s! SDL_mixer Error: %s\n", path, Mix_GetError());
    }
    return sound;
}

void audio_init(){

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    // 加载音效
    rotate_sound = Mix_LoadWAV("rotate.wav");
     if (rotate_sound == NULL) {
        printf("Failed to load rotate sound, SDL_mixer Error: %s\n", Mix_GetError());
    }
    move_sound = Mix_LoadWAV("move.wav");
     if (move_sound == NULL) {
        printf("Failed to load rotate sound, SDL_mixer Error: %s\n", Mix_GetError());
    }
    line_clear_sound = Mix_LoadWAV("clear_lines.wav");
    if (line_clear_sound == NULL) {
        printf("Failed to load line clear sound, SDL_mixer Error: %s\n", Mix_GetError());
    }
    down_sound = Mix_LoadWAV("clear_lines.wav");
    if (down_sound == NULL) {
        printf("Failed to load line clear sound, SDL_mixer Error: %s\n", Mix_GetError());
    }
    bg_sound = Mix_LoadMUS("bgm.mp3");
     if (bg_sound == NULL) {
        printf("Failed to load bgsound, SDL_mixer Error: %s\n", Mix_GetError());
    }
}

void audio_free(){
        // 释放音效资源
    if(rotate_sound){
        Mix_FreeChunk(rotate_sound);
        Mix_FreeChunk(line_clear_sound);
        Mix_FreeChunk(down_sound);
        Mix_FreeMusic(bg_sound);
        rotate_sound = NULL;
        line_clear_sound = NULL;
    }

    Mix_CloseAudio();
}

// 播放音效
void play_sound_rotate() {
    Mix_PlayChannel(-1, rotate_sound, 0);  // 播放音效，无需指定通道
}
void play_sound_move() {
    Mix_PlayChannel(-1, move_sound, 0);  // 播放音效，无需指定通道
}
void play_sound_clearline() {
    Mix_PlayChannel(-1, line_clear_sound, 0);  // 播放音效，无需指定通道
}
void play_sound_down() {
    Mix_PlayChannel(-1, down_sound, 0);  // 播放音效，无需指定通道
}
void play_sound_bgm(){
    Mix_PlayMusic(bg_sound, -1); 
}