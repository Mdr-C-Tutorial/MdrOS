#include "tty.h"
#include "video.h"

tty_t default_tty;

static void tty_print(tty_t *tty_d,const char* msg){
    vga_writestring(msg);
}

static void tty_move_cursor(tty_t *tty_d, int x, int y){
    vga_move_cursor(x,y);
}

void tty_init(void){
    default_tty.color = 0x1;
    default_tty.back_color = 0x0;
    default_tty.video_ram = (uint32_t *)0xB8000;

    default_tty.print = tty_print;
    default_tty.move_cursor = tty_move_cursor;
}