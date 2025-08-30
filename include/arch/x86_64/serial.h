#ifndef X86_64_SERIAL_H
#define X86_64_SERIAL_H

#include <stdbool.h>
#include <arch/x86_64/cpu.h>

void initSerial();
void writeSerial(const char* str);
void writeSerialChar(char c);
char readSerialChar();
bool isSerialTransmitEmpty();

#endif