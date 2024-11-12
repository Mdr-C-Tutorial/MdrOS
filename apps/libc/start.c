#include "syscall.h"

static inline int cmd_parse(char *cmd_str, char **argv, char token) {
    int arg_idx = 0;
    while (arg_idx < 50) {
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
        if (argc > 50) return -1;
        argc++;
    }

    return argc;
}

extern int main(int argc, char **argv);

void _start() {
    char *cmdline = (char *)__syscall0(SYSCALL_GET_ARG);
    int argc = 0;
    char *argv[50];
    argc = cmd_parse(cmdline, argv, ' ');
    int code = main(argc, argv);
    __syscall(SYSCALL_EXIT, code);
    while (1);
}