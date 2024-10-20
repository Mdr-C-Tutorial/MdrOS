#include "scheduler.h"
#include "page.h"
#include "krlibc.h"
#include "description_table.h"

extern void switch_to(struct context *prev, struct context *next); //asmfunc.asm

pcb_t *current_pcb = NULL;
pcb_t *running_proc_head = NULL;
bool can_sche = false; //调度标志位

void scheduler_process(registers_t *reg){
    if(current_pcb && can_sche){
        current_pcb->cpu_clock++;
        default_scheduler(reg,current_pcb->next);
    }
}

void default_scheduler(registers_t *reg,pcb_t* next){ //CP_Kernel 默认的进程调度器
    if (current_pcb != next) {
        pcb_t *prev = current_pcb;
        current_pcb = next;
        switch_page_directory(current_pcb->pgd_dir);
        set_kernel_stack((uintptr_t)current_pcb->kernel_stack + STACK_SIZE);

        prev->context.eip = reg->eip;
        prev->context.ds = reg->ds;
        prev->context.cs = reg->cs;
        prev->context.eax = reg->eax;
        prev->context.ss = reg->ss;
        switch_to(&(prev->context), &(current_pcb->context));
        reg->ds = current_pcb->context.ds;
        reg->cs = current_pcb->context.cs;
        reg->eip = current_pcb->context.eip;
        reg->eax = current_pcb->context.eax;
        reg->ss = current_pcb->context.ss;
    }
}