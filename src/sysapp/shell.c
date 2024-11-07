#include "shell.h"
#include "krlibc.h"
#include "klog.h"
#include "keyboard.h"
#include "cmos.h"

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

int gets(char *buf, int buf_size) {
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

int cmd_parse(char *cmd_str, char **argv, char token) {
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

        argc = cmd_parse(com, argv, ' ');

        if (argc == -1) {
            printk("[Shell]: Error: out of arguments buffer\n");
            continue;
        }


    }
}