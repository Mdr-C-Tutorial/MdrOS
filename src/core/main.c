#include "video.h"
#include "multiboot.h"
#include "description_table.h"
#include "io.h"
#include "tty.h"
#include "klog.h"

void kernel_main(multiboot_t *mboot){
    vga_install();

    if(mboot->cmdline != NULL){
        vga_writestring((char*)mboot->cmdline);
        vga_writestring("\n");
    }

    gdt_install();

    tty_init();

    printk("Hello! %s\n","MdrOS");

    while(1) io_hlt();
}