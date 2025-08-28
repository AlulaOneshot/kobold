#ifndef X86_64_SERIAL_H
#define X86_64_SERIAL_H

#include <stdint.h>

#define COM1 0x3F8

#define SERIAL_IRQ 4

void initSerial();
void writeSerial(uint8_t byte);
void writeSerialString(const char *str);
uint8_t readSerial();
int serialDebugLn(const char *format, ...);

#endif