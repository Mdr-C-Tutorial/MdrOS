#include "devfs.h"
#include "vfs.h"
#include "krlibc.h"

vfs_node_t devfs_sysinfo_node;

static void devfs_info_init(){
    vfs_mkdir("/dev/sys/name");
    char name[50];
    sprintf(name,"/dev/sys/name/%s",KERNEL_NAME);
    vfs_mkdir(name);
}

void devfs_sysinfo_init(){
    vfs_mkdir("/dev/sys");
    devfs_sysinfo_node = vfs_open("/dev/sys");

    devfs_info_init();
}