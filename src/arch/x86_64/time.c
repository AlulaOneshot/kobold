#include <arch/x86_64/time.h>
#include <arch/x86_64/cpu.h>

uint64_t ticks = 0;

void initialiseTime() {
    uint32_t divisor = 1193180 / TIMER_TIMEPERTICK;
    outb(0x43, 0x36); // Command byte: channel 0, lobyte/hibyte, mode 3, binary
    outb(0x40, divisor & 0xFF); // Low byte of divisor
    outb(0x40, (divisor >> 8) & 0xFF); // High byte of divisor
}

void timerTick(isr_registers_t *regs) {
    ticks++;
    sendEOI(0);
}

uint64_t getTime() {
    return ticks;
}

void sleep(uint64_t ms) {
    uint64_t target = getTime() + ms;
    while (getTime() < target) {
        asm volatile ("hlt"); // Sleep until the next interrupt
    }
}