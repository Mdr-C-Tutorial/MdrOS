#pragma once

enum pipfs_enum {
    PCB_ROOT,
    PCB_NAME,
};

struct pipfs_handle{ //PIPFS 句柄
    int pid;
    enum pipfs_enum type;
    char data[50];
};

typedef struct pipfs_handle* pipfs_handle_t;

void pipfs_init();