#pragma once

#define TASK_KERNEL_LEVEL 0
#define TASK_SYSTEM_SERVICE_LEVEL 1
#define TASK_APPLICATION_LEVEL 2

#include "ctypes.h"
#include "page.h"
#include "tty.h"
#include "vfs.h"

struct context{
    uint32_t esp;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ecx;
    uint32_t edx;
    uint32_t eflags;

    uint32_t eax;
    uint32_t eip;
    uint32_t ds;
    uint32_t cs;
    uint32_t ss;
};

typedef struct task_pcb{
    uint8_t task_level;           // 进程等级< 0:内核 | 1:系统服务 | 2:应用程序 >
    int pid;
    char* name;
    void* kernel_stack;           //内核栈
    void* program_break;          // 进程堆基址
    void* program_break_end;      // 进程堆尾
    page_directory_t *pgd_dir;    // 进程页表
    struct context context;       // 上下文信息
    tty_t *tty;                   // TTY设备
    bool fpu_flag;				  // 是否使用 FPU
    uint32_t cpu_clock;           // CPU运行时间片
    struct task_pcb *next;     // 链表指针
}pcb_t;

void init_pcb(uint32_t kernel_stack);