#include "draw.h"
#include "shape.h"
#include "board.h"
#include "control.h"
#include "audio.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	draw_init();
	shape_init();
	board_init();
	audio_init();
    control_gameloop();
	audio_free();
	draw_free();
    return 0;
}
