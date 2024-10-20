#include "pcb.h"
#include "kmalloc.h"
#include "klog.h"

extern pcb_t *current_pcb;
extern pcb_t *running_proc_head;
// scheduler.c

int now_pid;

void init_pcb(uint32_t kernel_stack){
    current_pcb = kmalloc(sizeof(pcb_t));

    current_pcb->task_level = TASK_KERNEL_LEVEL;
    current_pcb->pid = now_pid++;
    current_pcb->name = "CP_IDLE";
    current_pcb->next = current_pcb;
    current_pcb->kernel_stack = (void*)kernel_stack;

    extern void *program_break;
    extern void *program_break_end;
    current_pcb->program_break = program_break;
    current_pcb->program_break_end = program_break_end;

    running_proc_head = current_pcb;
    klogf(true,"Load task schedule. | KernelProcessName: %s PID: %d\n",current_pcb->name,current_pcb->pid);
}
