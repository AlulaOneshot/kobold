#ifndef X86_64_PANIC_H
#define X86_64_PANIC_H

#include <arch/x86_64/drivers/serial.h>
#include <khalt.h>

#define panic() serialDebugLn("[Kobold/Panic] Kernel Panic! at %s:%d", __FILE__, __LINE__); khalt();

#endif