#include <arch/x86_64/drivers/serial.h>
#include <arch/x86_64/cpu.h>
#include <spinlock.h>
#include <stdarg.h>
#include <string.h>

spinlock_t serialLock = SPINLOCK_INIT;

void initSerial() {
    outb(COM1 + 1, 0x00);    // Disable all interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB
    outb(COM1 + 0, 0x03);    // Set baud rate to 38400
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set

    outb(COM1 + 1, 0x01);    // Enable interrupts for the serial port

    serialDebugLn("[Kobold/Drivers/Serial] Serial Initialized");
}

void writeSerial(uint8_t byte) {
    while ((inb(COM1 + 5) & 0x20) == 0);
    outb(COM1, byte);
}

void writeSerialString(const char* str) {
    while (*str) {
        if (*str == '\n') {
            writeSerial('\r');
        }
        writeSerial(*str++);
    }
}

uint8_t readSerial() {
    while ((inb(COM1 + 5) & 1) == 0);
    return inb(COM1);
}

int serialDebugLn(const char *format, ...) {
    spinlockAcquire(&serialLock);
    
    va_list va;
    va_start(va, format);

    const char *p = format;
    char buf[32];
    while (*p) {
        if (*p == '%' && *(p + 1)) {
            p++;
            if (*p == 'd') {
                int val = va_arg(va, int);
                int i = 30, neg = 0;
                buf[31] = 0;
                if (val < 0) { neg = 1; val = -val; }
                do { buf[i--] = '0' + (val % 10); val /= 10; } while (val);
                if (neg) buf[i--] = '-';
                writeSerialString(&buf[i + 1]);
            } else if (*p == 'x') {
                unsigned int val = va_arg(va, unsigned int);
                int i = 30;
                buf[31] = 0;
                do {
                    int d = val & 0xF;
                    buf[i--] = d < 10 ? '0' + d : 'a' + d - 10;
                    val >>= 4;
                } while (val);
                writeSerialString(&buf[i + 1]);
            } else if (*p == 's') {
                char *s = va_arg(va, char*);
                writeSerialString(s ? s : "(null)");
            } else if (*p == 'c') {
                char c = (char)va_arg(va, int);
                writeSerial(c);
            } else {
                writeSerial('%');
                writeSerial(*p);
            }
            p++;
        } else {
            writeSerial(*p++);
        }
    }
    writeSerialString("\n");
    va_end(va);

    spinlockRelease(&serialLock);
    return strlen(format);
}