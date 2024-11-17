/*
 * Phigros Saturn Virtual OS
 * MdrOS Subsystem for SVOS
 */
#include "svos.h"
#include "scheduler.h"
#include "klog.h"

void svos_setup(){
    get_current_proc()->tty->clear(get_current_proc()->tty);
    printk("\033[?25l");

    // Open printk("\033[?25h");
    while (1);
}