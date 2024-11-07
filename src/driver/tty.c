#include "tty.h"
#include "video.h"
#include "krlibc.h"
#include "os_terminal.h"
#include "kmalloc.h"
#include "klog.h"

tty_t default_tty;

uint32_t tty_status = 0;

static void tty_print(tty_t *tty_d,const char* msg){
    size_t size = strlen(msg);
    if (tty_status == TTY_VGA_OUTPUT){
        for (size_t i = 0; i < strlen(msg); i++) vga_putchar(msg[i]);
    } else if (tty_status == TTY_OST_OUTPUT) {
        terminal_advance_state(msg);
    }
}

static void tty_putchar(tty_t *tty_d,int c){
    char buf[2] = {c,0};
    tty_print(tty_d, buf);
    if(tty_status == TTY_VGA_OUTPUT){
        vga_putchar(c);
    } else if(tty_status == TTY_OST_OUTPUT){
        terminal_advance_state_single(c);
    }
}

static void tty_move_cursor(tty_t *tty_d, int x, int y){
    vga_move_cursor(x,y);
}

tty_t* default_tty_alloc(){
    tty_t *tty = (tty_t*)kmalloc(sizeof(tty_t));
    tty->print = tty_print;
    tty->move_cursor = tty_move_cursor;
    tty->putchar = tty_putchar;
    strcpy(tty->name,"ost/default");

    tty->fifo = kmalloc(sizeof(struct FIFO8));
    char* buf = kmalloc(512);
    fifo8_init(tty->fifo, 512, (unsigned char *)buf);
    return tty;
}

void free_tty(tty_t *tty){
    kfree(tty->fifo);
    kfree(tty);
}

void tty_init(void){
    default_tty.color = 0x1;
    default_tty.back_color = 0x0;
    default_tty.video_ram = (uint32_t *)0xB8000;

    default_tty.print = tty_print;
    default_tty.move_cursor = tty_move_cursor;
    default_tty.putchar = tty_putchar;
    strcpy(default_tty.name,"ost/default");

    default_tty.fifo = kmalloc(sizeof(struct FIFO8));
    char* buf = kmalloc(512);
    fifo8_init(default_tty.fifo, 512, (unsigned char *)buf);

    tty_status = TTY_VGA_OUTPUT;
}