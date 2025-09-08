#include <arch/x86_64/dev.h>

void qemuDebugPutChar(char c) {
    asm volatile ("outb %0, $0xE9" : : "a"(c));
}

void _putchar (char c) {
    qemuDebugPutChar(c);
}