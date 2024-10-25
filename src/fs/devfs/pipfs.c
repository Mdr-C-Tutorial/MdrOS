#include "devfs.h"
#include "vfs.h"
#include "scheduler.h"
#include "klog.h"
#include "krlibc.h"

extern pcb_t *running_proc_head;

vfs_node_t proc_node;

void update_pipfs(){
    vfs_free_child(proc_node);
    pcb_t *fecur = running_proc_head;
    do{
        printk("Proc[%d] %s\n",fecur->pid,fecur->name);
        char proc_na[20];
        sprintf(proc_na,"/dev/sys/proc/%d",fecur->pid);

        vfs_mkdir(proc_na);

        fecur = fecur->next;
        if(fecur == NULL) break;
        if(fecur->pid == running_proc_head->pid) break;
    } while (1);
}

void pipfs_init(){
    vfs_mkdir("/dev/proc");
    proc_node = vfs_open("/dev/proc");
    update_pipfs();
}