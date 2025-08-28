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
static inline uint8_t inb(uint16_t _port) {
    uint8_t _data;
    asm volatile ("inb %1, %0" : "=a"(_data) : "Nd"(_port));
    return _data;
}

/// @brief Write one byte out to the specified port.
/// @param _port The port to write to.
/// @param _data The value to write.
static inline void outb(uint16_t _port, uint8_t _data) {
    asm volatile ("outb %0, %1" : : "a"(_data), "Nd"(_port));
}

/// @brief Waits a short amount of time, ususally to let an external device (i.e. keyboard, PIC, etc.) register an input. Done by outputing to the port 0x80 which is usally used to write to the motherboard's display, but is often unused after boot.
static inline void ioWait()
{
    outb(0x80, 0);
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
static inline uint64_t rdtsc(void)
{
    uint32_t low, high;
    asm volatile("rdtsc":"=a"(low),"=d"(high));
    return ((uint64_t)high << 32) | low;
}

/// @brief Read a value from CR0
/// @return The value of CR0
static inline unsigned long readCr0(void)
{
    unsigned long val;
    asm volatile ( "mov %%cr0, %0" : "=r"(val) );
    return val;
}

/// @brief Read a value from CR1
/// @return The value of CR1
static inline unsigned long readCr1(void)
{
    unsigned long val;
    asm volatile ( "mov %%cr1, %0" : "=r"(val) );
    return val;
}

static inline bool checkInterrupts() {
  uint16_t flags;
  asm volatile ( "pushf\n\t pop %0" : "=g"(flags) );
  return flags & (1 << 9);
}

#define IST_OFFSET_0 0b0000000000000000
#define IST_OFFSET_1 0b0000000000000001
#define IST_OFFSET_2 0b0000000000000010
#define IST_OFFSET_3 0b0000000000000011
#define INTERUPT_GATE 0b0000111000000000
#define TRAP_GATE 0b0000111100000000
#define IDT_RING_0 0b0000000000000000
#define IDT_RING_1 0b0010000000000000
#define IDT_RING_2 0b0100000000000000
#define IDT_RING_3 0b0110000000000000
#define IDT_PRESENT 0b1000000000000000

void setGateIDT(uint16_t gate, uint64_t base, uint16_t selector, uint8_t flags);
void initIDT();
void reloadIDT();

typedef struct idt_entry {
    uint16_t base0;
    uint16_t selector;
    uint16_t flags;
    uint16_t base1;
    uint32_t base2;
    uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

#endif