#include "video.h"
#include "multiboot.h"
#include "description_table.h"
#include "io.h"
#include "tty.h"
#include "klog.h"
#include "timer.h"
#include "acpi.h"
#include "page.h"
#include "free_page.h"
#include "pci.h"
#include "ide.h"
#include "vdisk.h"
#include "vfs.h"
#include "devfs.h"
#include "os_terminal.h"
#include "pcb.h"
#include "keyboard.h"
#include "scheduler.h"
#include "krlibc.h"
#include "syscall.h"
#include "shell.h"
#include "cpuid.h"

extern void* program_break_end;

extern void iso9660_regist(); //iso9660.c

_Noreturn void shutdown(){
    printk("Shutdown %s...\n",KERNEL_NAME);
    kill_all_proc();
    sleep(10);
    power_off();
    while (1);
}

/*
 * 内核初始化函数, 最终会演变为CPU0的IDLE进程
 * > 注意, 其所有的函数调用顺序均不可改变. 需按顺序初始化OS功能
 * @Noreturn
 */
_Noreturn void kernel_main(multiboot_t *multiboot, uint32_t kernel_stack) {
    vga_install();

    if(multiboot->cmdline != (multiboot_uint32_t)NULL) {
    }

    gdt_install();
    idt_install(); //8259A PIC初始化
    tty_init(); //tty 设备初始化

    init_vbe(multiboot);
    page_init(multiboot); //分页开启
    setup_free_page();
    terminal_setup();
    printk("MdrOS v0.0.1 %s (Limine Multiboot) on an i386\n",KERNEL_NAME);
    printk("KernelArea: 0x00000000 - 0x%08x | GraphicsBuffer: 0x%08x \n",
           program_break_end,
           multiboot->framebuffer_addr);
    init_cpuid();
    klogf(true, "Memory manager initialize.\n");
    acpi_install();  //ACPI初始化
    init_timer(1); //RTC 时钟中断
    vdisk_init();
    vfs_init();
    init_pci(); //pci设备列表加载, 所有PCI设备相关驱动初始化需在此函数后方调用

    ide_init();

    devfs_regist();
    io_cli(); //ide驱动会打开中断以加载硬盘设备, 需重新关闭中断以继续初始化其余OS功能
    iso9660_regist();
    init_pcb();
    keyboard_init();
    setup_syscall();
    create_kernel_thread((void*)setup_shell, NULL, "Shell");
    klogf(true,"Enable kernel shell service.\n");

    vfs_mkdir("/a");
    vfs_mount("/dev/ide0", vfs_open("/a"));

    klogf(true,"Kernel load done!\n");
    enable_scheduler();
    io_sti(); //内核加载完毕, 打开中断以启动进程调度器, 开始运行

    while(1){
        free_pages();
        io_hlt();
    }
}