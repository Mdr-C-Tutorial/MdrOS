#include "video.h"
#include "multiboot.h"
#include "description_table.h"
#include "io.h"
#include "tty.h"
#include "klog.h"
#include "timer.h"
#include "acpi.h"
#include "page.h"

_Noreturn void kernel_main(multiboot_t *multiboot){
    vga_install();

    if(multiboot->cmdline != (multiboot_uint32_t)NULL){
    }

    gdt_install();
    idt_install();
    tty_init();

    printk("CP_Kernel-i386_MDROS v0.0.1 (GRUB Multiboot) on an i386\n");
    printk("Memory Size: %dMB | ",(multiboot->mem_upper + multiboot->mem_lower) / 1024 + 1);
    printk("Video Resolution: %d x %d\n",multiboot->framebuffer_width,multiboot->framebuffer_height);

    init_timer(1);
    acpi_install();

    page_init(multiboot);

    printk("Hello! [nanoTime: %d] %s\n",nanoTime(),"MdrOS");

    while(1) io_hlt();
}