#pragma once

#include "ctypes.h"
#include "tty.h"
#include "video.h"
#include "kmalloc.h"
#include "klog.h"

bool terminal_init(unsigned int width,
                    unsigned int height,
                    uint32_t *screen,
                    void *(*malloc)(uintptr_t),
                    void (*free)(void*),
                    void (*serial_print)(const char*));
void terminal_destroy(void);
void terminal_flush(void);
void terminal_set_auto_flush(unsigned int auto_flush);
void terminal_advance_state(const char *s);
void terminal_advance_state_single(char c);
const char *terminal_handle_keyboard(unsigned char scancode);

static inline void terminal_setup(){
    extern uint32_t tty_status; tty_status = TTY_OST_OUTPUT;
    terminal_init(get_vbe_width(),get_vbe_height(),get_vbe_screen(),kmalloc,kfree,logk);
}