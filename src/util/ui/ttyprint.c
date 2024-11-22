#include "ttyprint.h"
#include "vfs.h"
#include "klog.h"
#include "krlibc.h"
#include "video.h"

#define STBTT_malloc(x,u)  ((void)(u),kmalloc(x))
#define STBTT_free(x,u)    ((void)(u),kfree(x))
#define STBTT_strlen(x)    strlen(x)
#define STBTT_memcpy       memcpy
#define STBTT_memset       memset
#define STBTT_fabs(x)      fabs(x)
#define STBTT_ifloor(x)   ((int) floor(x))
#define STBTT_iceil(x)    ((int) ceil(x))
#define STBTT_fmod(x,y)    fmod(x,y)
#define STBTT_cos(x)       cos(x)
#define STBTT_acos(x)      acos(x)
#define STBTT_sqrt(x)      sqrt(x)
#define STBTT_pow(x,y)     pow(x,y)
#define STBTT_assert(x)    assert(x)

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

void* load_font(const char* path){
    vfs_node_t file = vfs_open(path);
    if(file == NULL) return NULL;
    uint8_t *fontBuffer = kmalloc(file->size);
    vfs_read(file,fontBuffer,0,file->size);

    stbtt_fontinfo *info = kmalloc(sizeof(stbtt_fontinfo));
    if (!stbtt_InitFont(info, fontBuffer, 0)){
        goto error;
    }

    return info;
    error:
    printk("failed to load desktop ttf.\n");
    kfree(fontBuffer);
    kfree(info);
    return NULL;
}

void ttf_putchar(uint32_t *vram,void* info0,char c,int cx,int cy,int *width,int *height,int size){
    stbtt_fontinfo *info = info0;
    int b_w = 512; /* bitmap width */
    int b_h = 128; /* bitmap height */
    int l_h = 2 * size; /* line height */

    unsigned char* bitmap = kcalloc(b_w * b_h, sizeof(unsigned char));
    float scale = stbtt_ScaleForPixelHeight(info, l_h);

    int x = 0;

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(info, &ascent, &descent, &lineGap);

    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    int h = (int)((float)(ascent - descent + lineGap * scale));
    int max = 0;
    int i;
    int ax;
    int lsb;
    stbtt_GetCodepointHMetrics(info, c, &ax, &lsb);

    int c_x1, c_y1, c_x2, c_y2;
    stbtt_GetCodepointBitmapBox(info, c, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

    int y = ascent + c_y1;

    int byteOffset = x + roundf(lsb * scale) + (y * b_w);
    stbtt_MakeCodepointBitmap(info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, c);

    x += roundf(ax * scale);

    int kern;
    kern = stbtt_GetCodepointKernAdvance(info, c, c);
    x += roundf(kern * scale);

    *width = x;
    *height = max + h;
    put_bitmap(vram,bitmap,cx,cy,x,h,512,get_vbe_width(),0xc6c6c6);
    kfree(bitmap);
}

void ttf_print(uint32_t *vram,void *info0,char* data,int cx,int cy,int size){
    int x = cx;
    for (int i = 0; i < strlen(data); i++) {
        int width,height;
        ttf_putchar(vram,info0,data[i],x,cy,&width,&height,size);
        x += width;
    }
}