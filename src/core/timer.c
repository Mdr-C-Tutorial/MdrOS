#include "timer.h"
#include "isr.h"
#include "io.h"

uint32_t tick = 0;

static void timer_handle(registers_t *regs) {
    io_cli();
    tick++;
    //schedule(regs);
    io_sti();
}

void clock_sleep(uint32_t timer){
    uint32_t sleep = tick + timer;
    while(1){
        int a;
        if(tick >= sleep) break;
    }
}

void init_timer(uint32_t timer) {
    register_interrupt_handler(IRQ0, &timer_handle);
    uint32_t divisor = 1193180 / timer;

    outb(0x43, 0x36); // 频率

    uint8_t l = (uint8_t) (divisor & 0xFF);
    uint8_t h = (uint8_t) ((divisor >> 8) & 0xFF);

    outb(0x40, l);
    outb(0x40, h);
}
