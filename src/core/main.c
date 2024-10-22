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
#include "vfs.h"
#include "devfs.h"
#include "os_terminal.h"
#include "pcb.h"
#include "scheduler.h"

extern void* program_break_end;

int test_proc(void *pVoid){
    printk("Hello! World!\n");
    return 0;
}

/*
 * 内核初始化函数, 最终会演变为CPU0的IDLE进程
 * > 注意, 其所有的函数调用顺序均不可改变. 需按顺序初始化OS功能
 * @Noreturn
 */
_Noreturn void kernel_main(multiboot_t *multiboot,uint32_t kernel_stack){
    vga_install();

    if(multiboot->cmdline != (multiboot_uint32_t)NULL){
    }

    gdt_install();
    idt_install(); //8259A PIC初始化
    tty_init(); //tty 设备初始化

    init_timer(1); //RTC 时钟中断
    acpi_install();  //ACPI初始化

    init_vbe(multiboot);
    page_init(multiboot); //分页开启
    terminal_setup();

    printk("CP_Kernel-i386_MDROS v0.0.1 (GRUB Multiboot) on an i386\n");
    printk("KernelArea: %08x | GraphicsBuffer: %08x \n",program_break_end,multiboot->framebuffer_addr);
    klogf(true,"Memory manager initialize.\n");

    init_vdisk();
    vfs_init();
    init_pci(); //pci设备列表加载, 所有PCI设备相关驱动初始化需在此函数后方调用

    ide_init();
    io_cli(); //ide驱动会打开中断以加载硬盘设备, 需重新关闭中断以继续初始化其余OS功能
    devfs_regist();

    init_pcb(kernel_stack);

    creat_kernel_thread(test_proc,NULL,"TEST_PROC");

    klogf(true,"Kernel load done!\n");
    enable_scheduler();
    io_sti(); //内核加载完毕, 打开中断以启动进程调度器, 开始运行

    while(1) io_hlt();
}