#include "video.h"
#include "multiboot.h"
#include "description_table.h"
#include "io.h"
#include "tty.h"
#include "klog.h"
#include "timer.h"
#include "acpi.h"
#include "page.h"
#include "pci.h"
#include "ide.h"
#include "vdisk.h"
#include "os_terminal.h"

_Noreturn void kernel_main(multiboot_t *multiboot){ // 内核初始化函数, 最终会演变为CPU0的IDLE进程
    vga_install();

    if(multiboot->cmdline != (multiboot_uint32_t)NULL){
    }

    gdt_install();
    idt_install();
    tty_init();

    init_timer(1);
    acpi_install();

    init_vbe(multiboot);
    page_init(multiboot);
    terminal_setup();

    printk("CP_Kernel-i386_MDROS v0.0.1 (GRUB Multiboot) on an i386\n");

    init_vdisk();
    init_pci();

    ide_init();
    io_cli();

    io_sti();

    while(1) io_hlt();
}