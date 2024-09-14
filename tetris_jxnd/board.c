#include "board.h"
#include "draw.h"
#include "audio.h"

static Gameboard gb;

int board_getlevel(){
    return gb.level;
}
void board_init(){
    gb.level = 1;
    gb.clearlines = 0;
    for(int i=0; i < BOARD_H; i++){
        for(int j=0; j < BOARD_W; j++){
            gb.matrix[i][j] = (Block){{160,160,160,255},false};
        }
    }
}
void board_draw(){
    SDL_Rect rect = {0,0,BLOCKSIZE,BLOCKSIZE};
    for(int i=0; i < BOARD_H; i++){
        for(int j=0; j < BOARD_W; j++){
            if(gb.matrix[i][j].active){
                draw_rect(&rect,&gb.matrix[i][j].color);
            }
            rect.x += rect.w;
        }
        rect.x = 0;
        rect.y += rect.h;
    }

    char tempstring[32];
    SDL_Color color = {250,0,0,255};
    snprintf(tempstring,sizeof(tempstring),"Score:%d",gb.score);
    draw_string((BOARD_W+2)*BLOCKSIZE,7*BLOCKSIZE,tempstring,&color,30);
    color = (SDL_Color){250,255,0,255};
    snprintf(tempstring,sizeof(tempstring),"Level:%d",gb.level);
    draw_string((BOARD_W+2)*BLOCKSIZE,8*BLOCKSIZE+5,tempstring,&color,30);

    color = (SDL_Color){250,200,100,255};
    snprintf(tempstring,sizeof(tempstring),"Pause game press P");
    draw_string((BOARD_W+2)*BLOCKSIZE,10*BLOCKSIZE,tempstring,&color,12);

    color = (SDL_Color){250,200,100,255};
    snprintf(tempstring,sizeof(tempstring),"Pause game press P");
    draw_string((BOARD_W+2)*BLOCKSIZE,11*BLOCKSIZE+5,tempstring,&color,12);
    color = (SDL_Color){250,200,100,255};
    snprintf(tempstring,sizeof(tempstring),"Pause game press P");
    draw_string((BOARD_W+2)*BLOCKSIZE,12*BLOCKSIZE+10,tempstring,&color,12);
}

bool board_check_collision(int newx, int newy, Shape *shape){
    for(int i = 0; i < shape->size; i++){
        for(int j = 0; j < shape->size; j++){
            if(shape->matrix[i][j]){
                int x = newx + j;
                int y = newy + i;
                if(x < 0 || x >= BOARD_W || y >= BOARD_H)//碰边检测
                    return true;
                if(y > 0 && gb.matrix[y][x].active)//碰到已有图形
                    return true;
            }
        }
    }
    return false;
}
void board_place_shape(Shape *shape){
    for(int i=0; i<shape->size;i++){
        for(int j = 0; j < shape->size; j++){
            if(shape->matrix[i][j]){
                gb.matrix[shape->y+i][shape->x+j].active = true;
                gb.matrix[shape->y+i][shape->x+j].color = shape->color;
            }
        }
    }
}

void board_clear_full_lines(){
    int lines = 0;
    for(int y = 0; y < BOARD_H; y++){
        bool isfull = true;
        for(int i = 0; i < BOARD_W; i++){
            if(!gb.matrix[y][i].active){
                isfull = false;
                break;
            }
        }
        if(isfull){//检测到有满行
            play_sound_clearline();
            lines++;
            //printf("%d\n",y);
            for(int i = y; i > 0; i--){//满行以上的所有图形向下移动一行
                for(int j = 0; j < BOARD_W; j++){
                    gb.matrix[i][j] = gb.matrix[i-1][j];
                }
            }
            for(int j = 0; j < BOARD_W; j++){//清空最顶行
                gb.matrix[0][j].active = false;
            }
        }
    }
    switch(lines){
        case 1: gb.score+=100;break;
        case 2: gb.score+=300;break;
        case 3: gb.score+=500;break;
        case 4: gb.score+=800;break;
    }
    gb.clearlines += lines;
    if((gb.clearlines+1)%11 == 0){
        gb.level++;
    }
    printf("Score: %d\nLevel: %d\nClear: %d\n",gb.score,gb.level,gb.clearlines);
}