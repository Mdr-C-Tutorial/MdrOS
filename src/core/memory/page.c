#include "page.h"
#include "klog.h"
#include "isr.h"
#include "io.h"

extern uint32_t end; //linker.ld 内核末尾地址
volatile uint32_t *frames; //物理页框位图基址

uint32_t placement_address = (uint32_t)&end;

static void set_frame(uint32_t frame_addr) { //设置物理块为占用
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32_t frame_addr) { //释放物理块
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

uint32_t first_frame() { //获取第一个空闲物理块
    for (int i = 0; i < INDEX_FROM_BIT(0xFFFFFFFF / PAGE_SIZE); i++) {
        if (frames[i] != 0xffffffff) {
            for (int j = 0; j < 32; j++) {
                uint32_t toTest = 0x1 << j;
                if (!(frames[i] & toTest)) {
                    return i * 4 * 8 + j;
                }
            }
        }
    }
    return (uint32_t) - 1;
}

static void open_page(){ //打开分页机制
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}

void switch_page_directory(void *pdr){
    asm volatile("mov %0, %%cr3" : : "r"(pdr));
}

void page_fault(registers_t *regs) {
    io_cli();
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address)); //

    int present = !(regs->err_code & 0x1); // 页不存在
    int rw = regs->err_code & 0x2; // 只读页被写入
    int us = regs->err_code & 0x4; // 用户态写入内核页
    int reserved = regs->err_code & 0x8; // 写入CPU保留位
    int id = regs->err_code & 0x10; // 由取指引起

    printk("[ERROR]: Page fault |");
    if (present) {
        printk("Type: present;\n\taddress: %x  \n", faulting_address);
    } else if (rw) {
        printk("Type: read-only;\n\taddress: %x\n", faulting_address);
    } else if (us) {
        printk("Type: user-mode;\n\taddres: %x\n", faulting_address);
    } else if (reserved) {
        printk("Type: reserved;\n\taddress: %x\n", faulting_address);
    } else if (id) {
        printk("Type: decode address;\n\taddress: %x\n", faulting_address);
    }
}

uint32_t get_phy_memsize(multiboot_t *multiboot){
    uint32_t memsize = (multiboot->mem_upper + multiboot->mem_lower);
    if (memsize / 1024 + 1 < 3071) {
        printk("[kernel]: Minimal RAM amount for CP_Kernel is 3071 MB, but you have only %d MB.\n",
               (multiboot->mem_upper + multiboot->mem_lower) / 1024 + 1);
        while (1) io_hlt();
    } else return memsize * 1024;
}

void page_init(multiboot_t *multiboot){

    uint32_t nframes = 0xFFFFFFFF / PAGE_SIZE;



    uint32_t end_mem = get_phy_memsize(multiboot);
    printk("END MEM: %08x\n",end_mem);

    register_interrupt_handler(14, page_fault);
    //open_page();
}