#include <stdarg.h>
#include "klog.h"
#include "krlibc.h"
#include "tty.h"
#include "serial.h"
#include "video.h"

extern tty_t default_tty;

void logkf(char *formet,...){
    int len;
    va_list ap;
    va_start(ap, formet);
    char *buf[1024] = {0};
    len = vsprintf(buf, formet, ap);
    logk(buf);
    va_end(ap);
}

void logk(char *message){
    for (size_t i = 0; i < strlen(message); i++){
#ifndef OS_HARDWARE_MODE
        write_serial(message[i]);
#endif
    }
}

void printk(const char *formet, ...) {
    int len;
    va_list ap;
    va_start(ap, formet);
    char *buf[1024] = {0};
    len = vsprintf(buf, formet, ap);
    k_print(buf);
    va_end(ap);
}

void k_print(const char* message){
    default_tty.print(&default_tty,message);
}