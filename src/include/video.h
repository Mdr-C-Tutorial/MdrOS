#pragma once

#include "ctypes.h"


void vga_move_cursor();
void vga_clear();
void vga_putchar(char c);
void vga_write(const char *data, size_t size);
void vga_writestring(const char *data);
void vga_install(void);
