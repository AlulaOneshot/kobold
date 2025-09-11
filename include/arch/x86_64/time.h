#pragma once

#include <arch/x86_64/cpu.h>
#include <stdint.h>

#define TIMER_TIMEPERTICK 1000 // In Hz aka 1 Millisecond timer

void initialiseTime();

uint64_t getTime(); // Returns time in milliseconds since boot
void timerTick(isr_registers_t *regs);
void sleep(uint64_t ms);