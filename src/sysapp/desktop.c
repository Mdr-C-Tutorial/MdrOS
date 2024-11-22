#include "desktop.h"
#include "os_terminal.h"
#include "scheduler.h"
#include "klog.h"
#include "video.h"
#include "ttyprint.h"
#include "mouse.h"
#include "krlibc.h"

// Open Cur printk("\033[?25h");

uint32_t *buffer_graphics;

static void draw_rect(int x0, int y0, int x1, int y1, int c) {
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            (buffer_graphics)[y * get_vbe_width() + x] = c;
        }
    }
}

static void pane_clear(uint32_t color) {
    for (uint32_t i = 0; i < (get_vbe_width() * (get_vbe_height())); i++) {
        buffer_graphics[i] = color;
    }
}

static void replace_pane() {
    for (uint32_t i = 0; i < (get_vbe_width() * (get_vbe_height())); i++) {
        if (get_vbe_screen()[i] == buffer_graphics[i]) continue;
        get_vbe_screen()[i] = buffer_graphics[i];
    }
}

static void update_mouse_cur() {
    int x = get_mouse_x();
    int y = get_mouse_y();
    draw_rect(x, y, x + 30, y + 30, 0xc6c6c6);
}

void desktop_setup() {
    void *info = load_font("/a/IBM_VGA.TTF");
    if (info == NULL) printk("Error: Cannot load deskrop.\n");

    buffer_graphics = kmalloc(get_vbe_width() * get_vbe_height());

    get_current_proc()->tty->clear(get_current_proc()->tty);
    printk("\033[?25l");
    terminal_exit();
    vbe_clear(0x00ABFF);

    while (1) {
        pane_clear(0x00ABFF);
        ttf_print(buffer_graphics, info, "Welcome to MdrOS - Copyright by XIAOYI12.", 0, 0, 16);
        update_mouse_cur();
        replace_pane();
    }
}