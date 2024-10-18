#include "tty.h"
#include "video.h"
#include "krlibc.h"
#include "os_terminal.h"
#include "klog.h"

tty_t default_tty;

uint32_t tty_status = 0;

static void tty_putchar(tty_t *tty_d,int c){
    if(tty_status == TTY_VGA_OUTPUT){
        vga_putchar(c);
    } else if(tty_status == TTY_OST_OUTPUT){
        terminal_advance_state_single(c);
        logkf("%c",c);
    }
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

    tty_status = TTY_VGA_OUTPUT;
}