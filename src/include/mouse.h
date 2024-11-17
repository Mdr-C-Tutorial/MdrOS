#pragma once

#define MOUSE_IRQ    12
#define KB_DATA  0x60
#define KB_CMD   0x64
#define KB_ACK   0xfa
#define LED_CODE 0xed

#include "ctypes.h"

typedef struct {
    uint8_t buf[3], phase;
    int x, y, btn;
} MOUSE_DEC;

int get_mouse_x();
int get_mouse_y();
void mouse_init();