#include "shell.h"
#include "krlibc.h"
#include "klog.h"
#include "keyboard.h"
#include "cmos.h"
#include "vfs.h"
#include "pcb.h"

static inline int isprint_syshell(int c) {
    return (c > 0x1F && c < 0x7F);
}

static char getc() {
    char c;
    do{
        c = kernel_getch();
        if(c == '\b' || c == '\n') break;
    } while (!isprint_syshell(c));
    return c;
}

static int gets(char *buf, int buf_size) {
    int index = 0;
    char c;
    while ((c = getc()) != '\n') {
        if (c == '\b') {
            if (index > 0) {
                index--;
                printk("\b \b");
            }
        } else {
            buf[index++] = c;
            printk("%c",c);
        }
    }
    buf[index] = '\0';
    printk("%c",c);
    return index;
}

static int cmd_parse(char *cmd_str, char **argv, char token) {
    int arg_idx = 0;
    while (arg_idx < MAX_ARG_NR) {
        argv[arg_idx] = NULL;
        arg_idx++;
    }
    char *next = cmd_str;
    int argc = 0;

    while (*next) {
        while (*next == token) *next++;
        if (*next == 0) break;
        argv[argc] = next;
        while (*next && *next != token) *next++;
        if (*next) *next++ = 0;
        if (argc > MAX_ARG_NR) return -1;
        argc++;
    }

    return argc;
}

static void mkdir(int argc,char** argv){
    if (argc == 1) {
        printk("[Shell-MKDIR]: If there are too few parameters.\n");
        return;
    }
    if(vfs_mkdir(argv[1]) == -1){
        printk("Failed create directory [%s].\n",argv[1]);
    }
}

static void mount(int argc,char** argv){
    if (argc == 2) {
        printk("[Shell-MOUNT]: If there are too few parameters.\n");
        return;
    }
    vfs_node_t p = vfs_open(argv[1]);
    if(p == NULL) {
        printk("Cannot found mount directory.\n");
        return;
    }
    if(vfs_mount(argv[2],p) == -1){
        printk("Failed mount device [%s]\n",argv[2]);
    }
}

static void ls(int argc,char** argv){
    vfs_node_t p;
    if (argc == 1) {
        p = vfs_open("/");
    } else{
        p = vfs_open(argv[1]);
    }
    if(p == NULL){
        printk("Cannot fount directory.\n");
        return;
    }
    list_foreach(p->child, i) {
        vfs_node_t c = (vfs_node_t) i->data;
        printk("%s ", c->name);
    }
    printk("\n");
}

static void read(int argc,char** argv) {
    if (argc == 1) {
        printk("[Shell-MKDIR]: If there are too few parameters.\n");
        return;
    }
    vfs_node_t file = vfs_open(argv[1]);
    if(file != NULL){
        char* buf = kmalloc(file->size);
        if(vfs_read(file,buf,0,file->size) == -1){
            goto read_error;
        }
        for (int i = 0; i < file->size; i++) {
            printk("%c",buf[i]);
        }
        printk("\n");
    }
    read_error:
    printk("Cannot read file.\n");
}

static void shutdown_os(){
    shutdown();
}

static void print_help(){
    printk("Usage <command|app_path> [argument...]\n");
    printk("help h ?                 Get shell command help.\n");
    printk("mkdir     <name>         Make a directory to vfs.\n");
    printk("mount     <dir> <dev>    Mount a file system to vfs directory.\n");
    printk("ls        [path]         List all file or directory.\n");
    printk("read      <path>         Read a text file.\n");
    printk("shutdown                 Shutdown os.\n");
}

void setup_shell(){
    printk("Welcome to MdrOS (%s)\n"
           "\n"
           " * SourceCode:     https://github.com/Mdr-C-Tutorial/MdrOS\n"
           " * Website:        https://github.com/plos-clan\n"
           "\n"
           " System information as of %s \n"
           "\n"
           "  Users logged in:       Kernel\n"
           "\n"
           "Copyright 2024 XIAOYI12 (Build by GCC i686-elf-tools)\n"
            ,KERNEL_NAME
            ,get_date_time());
    char com[MAX_COMMAND_LEN];
    char *argv[MAX_ARG_NR];
    int argc = -1;
    while (1){
        printk("\033[32mKernel@localhost: \033[39m$ ");
        if (gets(com, MAX_COMMAND_LEN) <= 0) continue;
        char* com_copy[100];
        strcpy(com_copy,com);
        argc = cmd_parse(com, argv, ' ');

        if (argc == -1) {
            printk("[Shell]: Error: out of arguments buffer\n");
            continue;
        }

        if (!strcmp("help", argv[0]) || !strcmp("?", argv[0]) || !strcmp("h", argv[0])) {
            print_help();
        } else if(!strcmp("mkdir",argv[0]))
            mkdir(argc,argv);
        else if(!strcmp("ls",argv[0]))
            ls(argc,argv);
        else if(!strcmp("mount",argv[0]))
            mount(argc,argv);
        else if(!strcmp("read",argv[0]))
            read(argc,argv);
        else if(!strcmp("shutdown",argv[0]))
            shutdown_os();
        else{
            int pid;
            if((pid = create_user_process(argv[0],com_copy,"User",TASK_APPLICATION_LEVEL)) == -1)
                printk("\033[31m[Shell]: Unknown command '%s'.\033[39m\n", argv[0]);
            pcb_t *pcb;
            do{
                pcb = found_pcb(pid);
                if(pcb == NULL) break;
            } while (1);
            printk("\n");
        }
    }
}