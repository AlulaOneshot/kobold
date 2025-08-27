#ifndef X86_64_CPU_H
#define X86_64_CPU_H

#include <stdint.h>
#include <stdbool.h>

/// @brief Asks the CPU for info about it's features. Output is different depending on the inputs.
/// @param eax Whatever would be in the EAX register, goes here.
/// @param ebx Whatever would be in the EBX register, goes here.
/// @param ecx Whatever would be in the ECX register, goes here.
/// @param edx Whatever would be in the EDX register, goes here.
void cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

/// @brief Read one byte in from the specified port.
/// @param _port The port to read from.
/// @return The value read.
uint8_t inb(uint16_t _port);

/// @brief Write one byte out to the specified port.
/// @param _port The port to write to.
/// @param _data The value to write.
void outb(uint16_t _port, uint8_t _data);

/// @brief Waits a short amount of time, ususally to let an external device (i.e. keyboard, PIC, etc.) register an input. Done by outputing to the port 0x80 which is usally used to write to the motherboard's display, but is often unused after boot.
static inline void io_wait()
{
    outb(0x80, 0);
}

/// @brief Returns true if IRQs are enabled for the CPU
static inline bool are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

/// @brief Loads an IDT (interrupt descriptor table)
/// @param base The base address of the IDT to load
/// @param size The size of the IDT to load.
static inline void lidt(void* base, uint16_t size)
{
    // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };

    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}

/// @brief Read amount of clock ticks since last cpu reset.
/// @return The amount of clock ticks since the last cpu reset.
/// The code is weird bc of some gcc optimization bullshit.
/// Quote from osdev.wiki
/// 'On x86_64, the "A" constraint expects to write into the "rdx:rax" registers instead of "edx:eax". 
//  So GCC can in fact optimize the above code by not setting RDX at all.
//  You instead need to do it manually with bitshifting.'
uint64_t rdtsc(void)
{
    uint32_t low, high;
    asm volatile("rdtsc":"=a"(low),"=d"(high));
    return ((uint64_t)high << 32) | low;
}

/// @brief Read a value from CR0
/// @return The value of CR0
static inline unsigned long read_cr0(void)
{
    unsigned long val;
    asm volatile ( "mov %%cr0, %0" : "=r"(val) );
    return val;
}

/// @brief Read a value from CR1
/// @return The value of CR1
static inline unsigned long read_cr1(void)
{
    unsigned long val;
    asm volatile ( "mov %%cr1, %0" : "=r"(val) );
    return val;
}


#endif