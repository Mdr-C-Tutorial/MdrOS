#include "image.h"
#include "klog.h"
#include "video.h"
#include "vfs.h"
/*
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_THREAD_LOCALS
#define STBI_NO_STDIO
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#include "kmalloc.h"

void convert_ABGR_to_ARGB(uint32_t* bitmap, size_t num_pixels) {
    for (size_t i = 0; i < num_pixels; ++i) {
        uint32_t pixel = bitmap[i];
        uint8_t alpha = (pixel >> 24) & 0xFF;
        uint8_t red = (pixel >> 16) & 0xFF;
        uint8_t green = (pixel >> 8) & 0xFF;
        uint8_t blue = pixel & 0xFF;

        bitmap[i] = (alpha << 24) | (blue << 16) | (green << 8) | red;
    }
}

void draw_bitmap(int x,int y,int w,int h,uint8_t *buffer){
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            uint32_t *p = (uint32_t *) get_vbe_screen() + j * (uint32_t)get_vbe_screen() + i;
            *p = buffer[(j - y) * w + (i - x)];
        }
    }
}

void draw_image_xys(char* filename,uint32_t x,uint32_t y,uint32_t width,uint32_t height){
    vfs_node_t file = vfs_open(filename);
    if(file == NULL) return;
    uint8_t *buffer = kmalloc(file->size);
    vfs_read(file,buffer,0,file->size);

    int w, h, bpp;
    stbi_uc* b = stbi_load_from_memory(buffer,file->size, &w, &h, &bpp, 4);
    if(!b) {
        logk("[ERROR]: ");
        logkf("Fatal Error:%s\n",stbi_failure_reason());
        return;
    }
    uint8_t* b1 = kmalloc(width * height * 4);
    stbir_resize_uint8(b, w, h, 0, b1, width, height, 0, 4);
    convert_ABGR_to_ARGB(b1, width * height);
    draw_bitmap(x, y, 1280, 768, b1);

    vfs_close(file);
    kfree(buffer);
}
 */