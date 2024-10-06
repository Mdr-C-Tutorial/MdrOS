#include "video.h"
#include "multiboot.h"
#include "description_table.h"
#include "io.h"
#include "tty.h"
#include "klog.h"
#include "timer.h"
#include "acpi.h"

_Noreturn void kernel_main(multiboot_t *mboot){
    vga_install();

    if(mboot->cmdline != (multiboot_uint32_t)NULL){
        vga_writestring((char*)mboot->cmdline);
        vga_writestring("\n");
    }

    gdt_install();
    idt_install();
    tty_init();

    init_timer(1);
    acpi_install();

    printk("Hello! [nanoTime: %d] %s\n",nanoTime(),"MdrOS");

    while(1) io_hlt();
}