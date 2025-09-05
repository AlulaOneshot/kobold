#include <arch/x86_64/cpu.h>

static uint64_t ticks = 0;

void initTimer() {
    // Initialize PIT to interrupt at TIMER_TIMEPERTICK frequency
    uint32_t divisor = 1193180 / TIMER_TIMEPERTICK;
    outb(0x43, 0x36); // Command byte: channel 0, lobyte/hibyte, mode 3, binary
    outb(0x40, divisor & 0xFF); // Low byte of divisor
    outb(0x40, (divisor >> 8) & 0xFF); // High byte of divisor
}

void timerTick() {
    ticks++;
}

uint64_t getTime() {
    return (ticks * 1000) / TIMER_TIMEPERTICK; // Convert ticks to milliseconds
}

void sleep(uint64_t ms) {
    uint64_t targetTicks = ticks + (ms * TIMER_TIMEPERTICK) / 1000;
    while (ticks < targetTicks) {
        asm volatile("hlt"); // Halt CPU until next interrupt
    }
}