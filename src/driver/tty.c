#include "tty.h"
#include "video.h"
#include "krlibc.h"

tty_t default_tty;

static void tty_putchar(tty_t *tty_d,int c){

    vga_putchar(c);
}

static void tty_print(tty_t *tty_d,const char* msg){
    size_t size = strlen(msg);
    for (size_t i = 0; i < size; i++)
        tty_putchar(tty_d,msg[i]);
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
    default_tty.putchar = tty_putchar;
}