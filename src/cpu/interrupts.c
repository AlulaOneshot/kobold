#include <cpu.h>

void disableInterrupts() {
    #if ARCH_X86_64 == 1
    asm volatile ("cli");
    #endif
}

void enableInterrupts() {
    #if ARCH_X86_64 == 1
    asm volatile ("sti");
    #endif
}