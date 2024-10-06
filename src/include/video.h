#pragma once

#include "ctypes.h"


void vga_move_cursor(uint16_t x,uint16_t y);
void vga_clear();
void vga_putchar(char c);
void vga_write(const char *data, size_t size);
void vga_writestring(const char *data);
void vga_install(void);
