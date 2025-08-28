#include <stdarg.h>
#include <log.h>
#ifdef ARCH_X86_64
#include <arch/x86_64/drivers/serial.h>
#endif

void koboldLogLn(const char *str) {
    #ifdef ARCH_X86_64
        writeSerialString(str);
        writeSerialString("\n");
    #endif
}