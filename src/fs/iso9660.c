#include "iso9660.h"
#include "vfs.h"

static void dummy(){}

int isofs_mount(char* src, vfs_node_t node){

}

void isofs_open(void *parent, char* name, vfs_node_t node) {

}

int  isofs_read(void *file, void *addr, size_t offset, size_t size){

}

void isofs_close(){

}

static struct vfs_callback callbacks = {
        .mount   = isofs_mount,
        .unmount = (void *)dummy,
        .mkdir   = (void *)dummy,
        .mkfile  = (void *)dummy,
        .open    = isofs_open,
        .close   = isofs_close,
        .stat    = (void *)dummy,
        .read    = isofs_read,
        .write   = (void *)dummy,
};
