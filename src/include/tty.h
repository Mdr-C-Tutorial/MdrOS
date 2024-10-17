#pragma once

#define TTY_VGA_OUTPUT 2
#define TTY_VBE_OUTPUT 4
#define TTY_OST_OUTPUT 8

#include "ctypes.h"

typedef enum {
    MODE_A = 'A',
    MODE_B = 'B',
    MODE_C = 'C',
    MODE_D = 'D',
    MODE_E = 'E',
    MODE_F = 'F',
    MODE_G = 'G',
    MODE_H = 'H',
    MODE_f = 'f',
    MODE_J = 'J',
    MODE_K = 'K',
    MODE_S = 'S',
    MODE_T = 'T',
    MODE_m = 'm'
} vt100_mode_t;

typedef struct tty_device{
    void (*print)(struct tty_device *res,const char *string);
    void (*putchar)(struct tty_device *res,int c);
    void (*clear)(struct tty_device *res);
    void (*move_cursor)(struct tty_device *res,int x, int y);

    uint32_t volatile*video_ram; //显存基址
    uint32_t width,height;
    int x, y;
    uint32_t color, back_color;

    /* vt100 */
    int          vt100;       // 是否检测到标志
    char         buffer[81];  // 缓冲区
    int          buf_p;       // 缓冲区指针
    int          done;        // 这个东西读取完毕没有？
    vt100_mode_t mode;        // 控制模式
    int          color_saved; // 保存的颜色
    uint32_t     vt_status;      // 文本属性
}tty_t;

void tty_init(void);

