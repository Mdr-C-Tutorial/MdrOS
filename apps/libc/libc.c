#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "syscall.h"

void *malloc(size_t size) {
    return NULL; // TODO malloc implement
}

void *xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) exit(1);
    return ptr;
}

void free(void *ptr) {
    // TODO free implement
}

void *calloc(size_t n, size_t size) {
    void *ptr = malloc(n * size);
    if (ptr == NULL)
        return NULL;
    memset(ptr, 0, n * size);
    return ptr;
}

void exit(int retval) {
    __syscall(SYSCALL_EXIT, retval);
}

int putchar(char c) {
    return __syscall(SYSCALL_PUTC, c);
}

int puts(char *string) {
    return __syscall(SYSCALL_PRINT, string);
}

int printf(const char *fmt, ...) {
    static char buf[4096];
    va_list va;
    va_start(va, fmt);
    int rets = vsprintf(buf, fmt, va);
    va_end(va);
    puts(buf);
    return rets;
}