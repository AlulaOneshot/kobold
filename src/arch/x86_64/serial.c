// TEMPORARY SERIAL DRIVER FOR DEBUGGING PURPOSES
// TO BE REPLACED WITH A DEVICE-AS-FILE DRIVER LATER

#include <arch/x86_64/serial.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

void initSerial() {
    outb(0x3F8 + 1, 0x00);    // Disable all interrupts
    outb(0x3F8 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(0x3F8 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(0x3F8 + 1, 0x00);    //                  (hi byte)
    outb(0x3F8 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(0x3F8 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(0x3F8 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(0x3F8 + 1, 0x01);
}

void writeSerial(const char* str) {
    while (*str) {
        writeSerialChar(*str++);
    }
}

void writeSerialChar(char c) {
    while (isSerialTransmitEmpty() == 0);
    outb(0x3F8, c);
}

char readSerialChar() {
    while ((inb(0x3F8 + 5) & 1) == 0);
    return inb(0x3F8);
}

bool isSerialTransmitEmpty() {
    return inb(0x3F8 + 5) & 0x20;
}
// Implementation of _putchar used by printf
void _putchar(char character) {
    writeSerialChar(character);
}