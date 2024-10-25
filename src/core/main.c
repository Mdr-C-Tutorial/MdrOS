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
#include "keyboard.h"
#include "scheduler.h"
#include "krlibc.h"

extern void* program_break_end;

int test_proc(){
    while(1) printk("%c\n",kernel_getch());
    return -1;
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

    printk("MdrOS v0.0.1 %s (GRUB Multiboot) on an i386\n",KERNEL_NAME);
    printk("KernelArea: 0x00000000 - 0x%08x | GraphicsBuffer: 0x%08x \n",
           program_break_end,
           multiboot->framebuffer_addr);
    klogf(true,"Memory manager initialize.\n");

    init_vdisk();
    vfs_init();
    init_pci(); //pci设备列表加载, 所有PCI设备相关驱动初始化需在此函数后方调用

    ide_init();
    io_cli(); //ide驱动会打开中断以加载硬盘设备, 需重新关闭中断以继续初始化其余OS功能
    devfs_regist();

    init_pcb();
    keyboard_init();

    create_kernel_thread(test_proc,NULL,"Test");

    klogf(true,"Kernel load done!\n");
    enable_scheduler();
    io_sti(); //内核加载完毕, 打开中断以启动进程调度器, 开始运行
    /*
    int ret = vfs_mkdir("/dev");
    printk(" vfs  ret=%d\n",ret);
    vfs_mount(NULL, vfs_open("/dev"));
    vfs_node_t p = vfs_open("/");
    list_foreach(p->child, i) {
        vfs_node_t c = (vfs_node_t)i->data;
        printk("%s ", c->name);
    }
    printk("\n");

    uint8_t *buf = kmalloc(2048);
    p = vfs_open("/dev/ide_atapi0");
    vfs_read(p,buf,2048,2048);
    for(int i = 0;i<2048;i++) {
        printk("%02x ",buf[i]);
    }
     */

    while(1) io_hlt();
}