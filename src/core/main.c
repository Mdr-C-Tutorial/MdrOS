#include "video.h"
#include "multiboot.h"

void kernel_main(multiboot_t *mboot){
    vga_install();

    if(mboot->cmdline != NULL){
        vga_writestring((char*)mboot->cmdline);
        vga_writestring("\n");
    }
    vga_writestring("Hello! MdrOS!\n");

    while(1) asm("hlt");
}