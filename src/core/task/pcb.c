#include "pcb.h"
#include "kmalloc.h"
#include "klog.h"
#include "tty.h"
#include "io.h"
#include "scheduler.h"

extern pcb_t *current_pcb;
extern pcb_t *running_proc_head;
// scheduler.c

int now_pid;

extern void *program_break;
extern void *program_break_end;
extern page_directory_t *kernel_directory;
extern tty_t default_tty;

static void add_task(pcb_t *new_task){ //添加进程至调度链
    if(new_task == NULL) return;
    pcb_t *tailt = running_proc_head;
    while (tailt->next != running_proc_head) {
        tailt = tailt->next;
    }
    tailt->next = new_task;
}

_Noreturn static void process_exit(){
    register uint32_t eax asm("eax");
    printk("Kernel Process exit, Code: %d\n",eax);
    kill_proc(current_pcb);
    while (1);
}

int creat_kernel_thread(int (*_start)(void* arg),void *args,char* name){ //创建内核进程 (内核线程)
    io_cli();
    pcb_t *new_task = kmalloc(STACK_SIZE);
    memset(new_task, 0, sizeof(pcb_t));

    new_task->task_level = TASK_KERNEL_LEVEL;
    new_task->name = name;
    new_task->pid = now_pid++;
    new_task->program_break = program_break;
    new_task->program_break_end = program_break_end;
    new_task->pgd_dir = kernel_directory;
    new_task->cpu_clock = 0;
    new_task->tty = &default_tty;
    new_task->cpu_clock = 0;

    new_task->kernel_stack = new_task;

    uint32_t *stack_top = (uint32_t * )((uint32_t) new_task + STACK_SIZE);
    *(--stack_top) = (uint32_t) args;
    *(--stack_top) = (uint32_t) process_exit;
    *(--stack_top) = (uint32_t) _start;

    new_task->context.esp = (uint32_t) new_task + STACK_SIZE - sizeof(uint32_t) * 3;
    new_task->context.eflags = 0x200;

    add_task(new_task);
    io_sti();
    return new_task->pid;
}

void kill_proc(pcb_t *pcb){
    io_cli();
    if (pcb->pid == 0) {
        klogf(false,"Cannot kill kernel processes.\n");
        io_sti();
        return;
    }

    if(pcb->task_level == TASK_KERNEL_LEVEL){

    }

    pcb_t *head = running_proc_head;
    pcb_t *last = NULL;
    while (1) {
        if (head->pid == pcb->pid) {
            last->next = pcb->next;
            kfree(pcb);
            io_sti();
            return;
        }
        last = head;
        head = head->next;
    }
}

pcb_t *found_pcb(int pid){
    pcb_t *l = running_proc_head;
    while (l->pid != pid){
        l = l->next;
        if(l == NULL || l == running_proc_head) return NULL;
    }
    return l;
}

void init_pcb(uint32_t kernel_stack){
    current_pcb = kmalloc(sizeof(pcb_t));

    current_pcb->task_level = TASK_KERNEL_LEVEL;
    current_pcb->pid = now_pid++;
    current_pcb->name = "CP_IDLE";
    current_pcb->next = current_pcb;
    current_pcb->kernel_stack = current_pcb;
    current_pcb->tty = &default_tty;
    current_pcb->pgd_dir = kernel_directory;
    current_pcb->context.esp = (uint32_t )current_pcb->kernel_stack;

    current_pcb->program_break = program_break;
    current_pcb->program_break_end = program_break_end;

    running_proc_head = current_pcb;
    klogf(true,"Load task schedule. | KernelProcessName: %s PID: %d\n",current_pcb->name,current_pcb->pid);
}
