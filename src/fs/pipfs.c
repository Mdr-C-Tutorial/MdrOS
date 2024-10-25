#include "pipfs.h"
#include "vfs.h"
#include "scheduler.h"
#include "klog.h"
#include "krlibc.h"

extern pcb_t *running_proc_head;
int pipfs_id = 0;
vfs_node_t proc_node;

static void dummy() {}

void update_pipfs(){
    vfs_free_child(proc_node);
    pcb_t *fecur = running_proc_head;
    do{
        char proc_na[20];
        sprintf(proc_na,"/proc/%d",fecur->pid);
        vfs_mkdir(proc_na);

        fecur = fecur->next;
        if(fecur == NULL) break;
        if(fecur->pid == running_proc_head->pid) break;
    } while (1);

    vfs_node_t p = vfs_open("/proc");
    list_foreach(p->child, i) {
        vfs_node_t c = (vfs_node_t)i->data;
        printk("Proc[%d] \n", c->name);
    }
}

static int pipfs_mount(char* src, vfs_node_t node){
    update_pipfs();

    return 0;
}

static void pipfs_open(void *parent, char* name, vfs_node_t node) {
    node->type   = file_virtual;
    node->size   = 0;
}

static void pipfs_close(void *current){
}

static int pipfs_read(void *file, void *addr, size_t offset, size_t size) {
    if(addr == NULL) return -1;
    int pid = (int) file;

    return 0;
}

static struct vfs_callback callbacks = {
        .mount   = pipfs_mount,
        .unmount = (void *)dummy,
        .mkdir   = (void *)dummy,
        .mkfile  = (void *)dummy,
        .open    = pipfs_open,
        .close   = pipfs_close,
        .stat    = (void *)dummy,
        .read    = pipfs_read,
        .write   = (void *)dummy,
};

void pipfs_init(){
    pipfs_id = vfs_regist("pipfs",&callbacks);
    vfs_mkdir("/proc");
    proc_node = vfs_open("/proc");
    vfs_mount(NULL,proc_node);
}