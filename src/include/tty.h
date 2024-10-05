#ifndef MDROS_TTY_H
#define MDROS_TTY_H

#include "ctypes.h"

typedef struct tty_device{
    void (*print)(struct tty_device *res,const char *string);
    void (*putchar)(struct tty_device *res,int c);
    void (*clear)(struct tty_device *res);

    uint32_t volatile*video_ram; //显存基址
    uint32_t width,height;
    int x, y;
    uint32_t color, back_color;

}tty_t;

#endif
