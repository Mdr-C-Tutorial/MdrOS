#include "video.h"

uint32_t width;
uint32_t height;
uint32_t *screen;

static video_area_t videoArea;

void vbe_clear(uint32_t color) {
  width = videoArea.width;
  height = videoArea.height;
  screen = videoArea.screen;
  for (uint32_t i = 0; i < (width * (height)); i++) {
    screen[i] = color;
  }
}

uint32_t *get_vbe_screen() { return videoArea.screen; }

uint32_t get_vbe_width() { return videoArea.width; }

uint32_t get_vbe_height() { return videoArea.height; }

void init_vbe(multiboot_t *multiboot) {
  screen = videoArea.screen = (uint32_t *)multiboot->framebuffer_addr;
  width = videoArea.width = multiboot->framebuffer_width;
  height = videoArea.height = multiboot->framebuffer_height;
  vbe_clear(0xc6c6c6);
}
