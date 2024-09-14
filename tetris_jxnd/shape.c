#include "shape.h"
#include "draw.h"
#include "board.h"
#include "control.h"
#include "audio.h"

const Shape ALL_SHAPES[7] = {{{{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},3,0,3,{160,160,160,255}},//L
{{{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}},3,0,3,{180,180,100,255}}, //J
{{{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},3,0,3,{100,160,100,255}}, //T
{{{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},3,0,2,{160,160,10,255}},  //O
{{{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}},3,0,4,{160,160,160,255}}, //I
{{{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},3,0,3,{10,160,10,255}},   //S
{{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},3,0,3,{160,10,160,255}}}; //Z
//作业1：完成所有形状的定义
Shape cur_shape,next_shape,temp_shape;
int delay_cnt;


// Queue to determine the next tetromino.
// Knuth shuffle algorithm is applied.
static uint8_t tetromino_queue[7];
static uint8_t tetromino_queue_size = 7;
static uint8_t current_queue_index = 0;


int rrand(int m) {
    return (int)((double)m * ( rand() / (RAND_MAX+1.0) ));
}
#define BYTE(X) ((unsigned char *)(X))

void shuffle(void *obj, size_t nmemb, size_t size) {
    void *temp = malloc(size);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    unsigned char *ptr = (unsigned char *)obj;
    size_t n = nmemb;
    while ( n > 1 ) {
        size_t k = rrand(n--);//生成一个随机索引k，范围从0到n-1。
        memcpy(temp, ptr + n*size, size);//将当前元素复制到临时缓冲区
        memcpy(ptr + n*size, ptr + k*size, size);//将随机选择的元素复制到当前位置。
        memcpy(ptr + k*size, temp, size);//将临时缓冲区的元素复制到随机选择的位置。
    }
    free(temp);
}


void shape_init(){


    int i;
    // build tetromino queue
    current_queue_index = 0;
    i = tetromino_queue_size;
    int n = 0;
    while(i --> 0) {
        tetromino_queue[i] = i;
    }
    //0,1,2,3,4,5,6
    shuffle(tetromino_queue, tetromino_queue_size, sizeof(uint8_t));
    //经过洗牌后的乱序


    cur_shape = ALL_SHAPES[tetromino_queue[current_queue_index++]];
    next_shape = ALL_SHAPES[tetromino_queue[current_queue_index++]];
    
    delay_cnt = 0;
}
//Day4作业：在界面显示预览图形块
static void shape_draw_matrix(int x, int y, Shape *shape){
    SDL_Rect r = {0,0,30,30};
    for(int i=0; i<shape->size; i++){//0-15
        for(int j=0; j<shape->size; j++){//0-9
            if(shape->matrix[i][j]){
                r.x = (x+shape->x+j)*30;
                r.y = (y+shape->y+i)*30;
                draw_rect(&r,&shape->color);
            }
        }
    }
}
void shape_draw(){
    shape_draw_matrix(0,0,&cur_shape);
    shape_draw_matrix(10,2,&next_shape);
    if(control_getstate() == STATE_RUNNING){
        if(delay_cnt++ > 30-3*board_getlevel()){
            delay_cnt = 0;
            shape_move(MOVE_DOWN);
        }
    }
    
    //printf("%d,%d\n",cur_shape.x,cur_shape.y);
}
//作业：实现下落块的旋转
void shape_rotate(Shape *shape){
    int size = shape->size;
    char matrix[size][size];
    for(int i=0; i < size; i++){
        for(int j = 0; j < size; j++){
            matrix[i][j] = shape->matrix[i][j];
        }
    }
    //memcpy
    for(int i=0; i < size; i++){
        for(int j = 0; j < size; j++){
            shape->matrix[i][j] = matrix[size-1-j][i];
        }
    }
}
void shape_move(Movedir dir){
    switch(dir){
        case MOVE_DOWN:
            if(!board_check_collision(cur_shape.x,cur_shape.y+1,&cur_shape)){
                cur_shape.y++;
                //play_sound_down();
            }
            else{
                board_place_shape(&cur_shape);
                board_clear_full_lines();
                cur_shape = next_shape;
                next_shape = ALL_SHAPES[tetromino_queue[current_queue_index++]];
                if(current_queue_index >= tetromino_queue_size){
                    current_queue_index = 0;
                    shuffle(tetromino_queue, tetromino_queue_size, sizeof(uint8_t));
    //经过洗牌后的乱序
                }
                if(board_check_collision(cur_shape.x,cur_shape.y,&cur_shape)){
                    control_setstate(STATE_GAME_OVER);
                    printf("Game Over!\n");
                }
            }
        break;
        case MOVE_LEFT:
            if(!board_check_collision(cur_shape.x-1,cur_shape.y,&cur_shape)) {
                cur_shape.x--;
                play_sound_move();
            }
        break;
        case MOVE_RIGHT:
            if(!board_check_collision(cur_shape.x+1,cur_shape.y,&cur_shape)) {
                cur_shape.x++;
                play_sound_move();
            }
        break;
        case MOVE_UP:
            temp_shape = cur_shape;
            shape_rotate(&temp_shape);//作业：完成图形块的旋转
            if(!board_check_collision(temp_shape.x,temp_shape.y,&temp_shape)) {
                shape_rotate(&cur_shape);
                play_sound_rotate();
            }
        break;
    }

}

