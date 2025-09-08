#include <cpu.h>
#if ARCH_X86_64 == 1
    #include <arch/x86_64/cpu.h>
#endif

void initialiseInterrupts() {
    #if ARCH_X86_64 == 1
        initialiseIDT();
    #endif
}