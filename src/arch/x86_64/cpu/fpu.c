#include <arch/x86_64/cpu.h>
#include <arch/x86_64/printf.h>

void initFPU() {
    if (!cpuidFeature(X86_64_CPUID_FEATURE_FXSR)) {
        // FXSAVE/FXRSTOR not supported, cannot use FPU
        printf("FPU not supported on this CPU!\n");
        printf("Halting...\n");
        while (1);
        return;
    }
    
    // Setup FPU emulation And Coprocessor Monitoring
    uint64_t cr0 = getCR0();
    cr0 |= (1 << 2);  // Set EM (Emulation) bit to 0
    cr0 |= (1 << 1);  // Set MP (Monitor Coprocessor) bit to 1
    setCR0(cr0);

    uint64_t cr4 = getCR4();
    cr4 |= (1 << 9);  // Set OSFXSR (Operating System Support for FXSAVE and FXRSTOR instructions) bit to 1
    cr4 |= (1 << 10); // Set OSXMMEXCPT (Operating System Support for Unmasked SIMD Floating-Point Exceptions) bit to 1
    setCR4(cr4);

    if (cpuidFeature(X86_64_CPUID_FEATURE_XSAVE)) { // Enable XSAVE if supported
        cr4 = getCR4();
        cr4 |= (1 << 18); // Set OSXSAVE (Operating System Support for XSAVE and Processor Extended States) bit to 1
        setCR4(cr4);

        uint64_t xcr0 = 0;
        xcr0 |= 1;      // Enable x87 state
        xcr0 |= (1 << 1); // Enable SSE state
        if (cpuidFeature(X86_64_CPUID_FEATURE_AVX)) {
            xcr0 |= (1 << 2); // Enable AVX state
        }
        if (cpuidFeature(X86_64_CPUID_FEATURE_AVX512)) {
            xcr0 |= (1 << 5); // Enable AVX-512 state
            xcr0 |= (1 << 6); // Enable AVX-512 state
            xcr0 |= (1 << 7); // Enable AVX-512 state
        }
        asm volatile("xsetbv" : : "a"(xcr0), "d"(xcr0 >> 32), "c"(0) : "memory");
    }
}