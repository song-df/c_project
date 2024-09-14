#include "draw.h"
#include "shape.h"
#include "board.h"

static Renderer renderer;

void disableIME(SDL_Window *window) {
    #if 0 //在Windows中把这个0变成1
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);// 初始化 wmInfo 结构体的版本信息
    if (SDL_GetWindowWMInfo(window, &wmInfo)) {
        HWND hwnd = wmInfo.info.win.window;
        // 禁用 IME
        ImmAssociateContext(hwnd, NULL);
    }
    #endif
}


bool draw_init(){
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    // 创建一个窗口
    renderer.window = SDL_CreateWindow("Hello SDL", 
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 
                                       640, 480, 
                                       SDL_WINDOW_SHOWN);
    if (renderer.window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // 创建一个渲染器
    renderer.renderer = SDL_CreateRenderer(renderer.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer.renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(renderer.window);
        SDL_Quit();
        return false;
    }
    disableIME(renderer.window);


    if(TTF_Init() < 0) {
        printf("Failed to initialize SDL_ttf\n");
        draw_free();
        return false;
    }
    renderer.font_30 = TTF_OpenFont("OPPOSans-M.ttf",30);
    if(!renderer.font_30){
        printf("Failed to load font: %s\n",TTF_GetError());
        TTF_Quit();
        draw_free();
        return false;
    }
    renderer.font_12 = TTF_OpenFont("OPPOSans-M.ttf",20);
    if(!renderer.font_12){
        printf("Failed to load font: %s\n",TTF_GetError());
        TTF_Quit();
        draw_free();
        return false;
    }

    // Load the background BMP image
    SDL_Surface* bgSurface = SDL_LoadBMP("background.bmp");
    if (!bgSurface) {
        printf("Failed to load background image: %s\n", SDL_GetError());
        exit(1);
    }
    
    renderer.background = SDL_CreateTextureFromSurface(renderer.renderer, bgSurface);
    SDL_FreeSurface(bgSurface);  // Free the surface after creating the texture
    if (!renderer.background) {
        printf("Failed to create background texture: %s\n", SDL_GetError());
        exit(1);
    }


    printf("draw init success!\n");
    return true;
}
void draw_free(){
    // 清理并退出
    if(renderer.font_30){
        TTF_CloseFont(renderer.font_30);
        TTF_CloseFont(renderer.font_12);
        TTF_Quit();
    }
    SDL_DestroyRenderer(renderer.renderer);
    SDL_DestroyWindow(renderer.window);
    SDL_Quit();
}

void draw_string(int x, int y, const char *str, SDL_Color *color, int s){
    SDL_Surface* surface = TTF_RenderUTF8_Solid(s==30?renderer.font_30:renderer.font_12, str,*color);
    if(!surface){
        printf("TTF_RenderText_Solid error: %s\n",TTF_GetError());
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.renderer,surface);
    if(!texture){
        printf("SDL_CreateTextureFromSurFace error:%s\n",SDL_GetError());
        return;
    }

    SDL_Rect rect = {x, y, surface->w,surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer.renderer,texture,NULL,&rect);   
    SDL_DestroyTexture(texture);
}

void draw_rect(const SDL_Rect *rect, const SDL_Color *color){
    SDL_Rect r = *rect;
	SDL_SetRenderDrawColor(renderer.renderer,color->r,color->g,color->b,color->a);
	SDL_RenderFillRect(renderer.renderer,rect);
    SDL_SetRenderDrawColor(renderer.renderer,255,255,255,255);
	SDL_RenderDrawRect(renderer.renderer,&r);
    // r.x++;r.y++;r.w-=2;r.h-=2;
	// SDL_RenderDrawRect(renderer.renderer,&r);
    // r.x++;r.y++;r.w-=2;r.h-=2;
	// SDL_RenderDrawRect(renderer.renderer,&r);
//作业1，完成一个带边框的矩形，边框线条3个像素，边框线条可指定不同颜色。
}	
SDL_Rect rect = {100,100,30,30};
SDL_Color color = {255,0,0,255};

//作业2，画一个高16个，宽10个的网格，网格间隔为30. 
//扩展，整个网格有更粗的边。
void draw_crossboard(int width, int height, int size){
    SDL_SetRenderDrawColor(renderer.renderer, 250, 250, 250, 255);  // 灰色
    for(int i = 0; i <= width; i++){
        SDL_RenderDrawLine(renderer.renderer,i*30,0,i*30,height*30);
        if(i==0) SDL_RenderDrawLine(renderer.renderer,i*30+1,0,i*30+1,height*30);
        if(i==width) SDL_RenderDrawLine(renderer.renderer,i*30-1,0,i*30-1,height*30);
    }
    for(int i = 0; i <= height; i++){
        SDL_RenderDrawLine(renderer.renderer,0,i*30,width*30,i*30);
        if(i == 0) SDL_RenderDrawLine(renderer.renderer,0,i*30+1,width*30,i*30+1);
        if(i == height) SDL_RenderDrawLine(renderer.renderer,0,i*30-1,width*30,i*30-1);
    }
}
void draw_update(){
	    // 设置渲染器颜色（蓝色）
    SDL_SetRenderDrawColor(renderer.renderer, 0, 0, 255, 255);
    // 清空渲染器
    SDL_RenderClear(renderer.renderer);

     SDL_RenderCopy(renderer.renderer, renderer.background, NULL, NULL);//显示背景图片
    //整个界面更新数据都是在这里
	//draw_rect(&rect,&color);
    draw_crossboard(10,16,30);
    board_draw();
    shape_draw();

    // 更新屏幕显示
    SDL_RenderPresent(renderer.renderer);

}
