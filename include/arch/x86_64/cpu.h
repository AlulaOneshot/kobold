#ifndef X86_64_CPU_H
#define X86_64_CPU_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
    The Kobold Kernel
    cpu.h - Pretty much everything CPU related for x86_64.
    Includes:
        GDT
        IDT
        CPU instructions
        FPU
        Timer (With PIT, HPET, whatever)
        PCI
        ACPI
        Stack
*/

// GDT Stuff

// Access Byte Flags
#define ACCESS_ACCESSED (1 << 0)
#define ACCESS_READ_WRITE (1 << 1) // Readable for code, writable for data
#define ACCESS_CONFORMING (1 << 2) // Conforming for code, expand down
#define ACCESS_EXECUTABLE (1 << 3) // 1 for code, 0 for data
#define ACCESS_DESCRIPTOR_TYPE (1 << 4) // 0 for system, 1 for code/data
#define ACCESS_PRIVILEGE_RING0 (0 << 5)
#define ACCESS_PRIVILEGE_RING1 (1 << 5)
#define ACCESS_PRIVILEGE_RING2 (2 << 5)
#define ACCESS_PRIVILEGE_RING3 (3 << 5)
#define ACCESS_PRESENT (1 << 7)

// Flags
#define FLAGS_LONG_MODE_CODE (1 << 5) // 64-bit code segment
#define FLAGS_32_BIT (1 << 6) // 32-bit segment (0 for 64-bit code segment).
#define FLAGS_GRANULARITY (1 << 7) // Limit scaled by 4K when set.

/// @brief A GDT entry.
typedef struct {
    uint16_t limit0; // Limit 0:15
    uint16_t base0; // Base 0:15
    uint8_t base1; // Base 16:23
    uint8_t access; // Access Byte
    uint8_t limit1_flags; // Limit 16:19 and flags
    uint8_t base2; // Base 24:31
} __attribute__((packed)) gdt_entry_t;

/// @brief A TSS descriptor entry.
typedef struct {
    uint16_t limit0; // Limit 0:15
    uint16_t base0; // Base 0:15
    uint8_t base1; // Base 16:23
    uint8_t access; // Access Byte
    uint8_t limit1_flags; // Limit 16:19 and flags
    uint8_t base2; // Base 24:31
    uint32_t base3; // Base 32:63
    uint32_t reserved;
} __attribute__((packed)) tss_descriptor_entry_t;

/// @brief A Task State Segment (TSS).
typedef struct {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t io_map_base;
} __attribute__((packed)) tss_t;

/// @brief The GDT itself.
typedef struct {
    gdt_entry_t null;
    gdt_entry_t kernel_code;
    gdt_entry_t kernel_data;
    gdt_entry_t user_code;
    gdt_entry_t user_data;
    tss_descriptor_entry_t tss_desc;
} __attribute__((packed)) gdt_t;

/// @brief A pointer to the GDT, for use with the lgdt instruction.
typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_pointer_t;

#define makeSegmentSelector(index, table, rpl) (uint16_t)(((uint16_t)index << 3) | ((bool)(table & 0b1) << 2) | ((uint8_t)(rpl & 0b11)))

/// @brief Initialize the TSS and the GDT.
void initGDT();
/// @brief Load the GDT into the CPU.
void reloadGDT();

// IDT Stuff

#define IDT_NO_IST 0b000
#define IDT_IST_1 0b001
#define IDT_IST_2 0b010
#define IDT_IST_3 0b011
#define IDT_IST_4 0b100
#define IDT_IST_5 0b101
#define IDT_IST_6 0b110
#define IDT_IST_7 0b111
#define IDT_TYPE_INTERRUPT_GATE 0b1110
#define IDT_TYPE_TRAP_GATE 0b1111
#define IDT_TYPE_DPL0 (0b00 << 5)
#define IDT_TYPE_DPL1 (0b01 << 5)
#define IDT_TYPE_DPL2 (0b10 << 5)
#define IDT_TYPE_DPL3 (0b11 << 5)
#define IDT_PRESENT (0b1 << 7)

/// @brief A pointer to the IDT, for use with the lidt instruction.
typedef struct {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) idt_descriptor_t;

/// @brief An IDT entry.
typedef struct {
    uint16_t offset0; // Offset 0:15
    uint16_t selector; // A code segment selector in GDT or LDT
    uint8_t ist; // Bits 0-2 hold Interrupt Stack Table offset, rest of bits zero.
    uint8_t type_attr; // Type and attributes
    uint16_t offset1; // Offset 16:31
    uint32_t offset2; // Offset 32:63
    uint32_t reserved; // Reserved
} __attribute__((packed)) idt_entry_t;

/// @brief CPU register state pushed by the ISR.
typedef struct {
  uint64_t ds; // & es

  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t rbp;
  uint64_t rdi;
  uint64_t rsi;
  uint64_t rdx;
  uint64_t rcx;
  uint64_t rbx;
  uint64_t rax;

  uint64_t interrupt;
  uint64_t error;

  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
} isr_registers_t;

/// @brief Initialize the IDT.
void initIDT();
/// @brief Set an entry in the IDT.
/// @param vector The entry vector to set (0-255).
/// @param isr The ISR function pointer.
/// @param selector The segment to load into CS.
/// @param ist The interrupt stack table to use, or 0 for none.
/// @param type_attr The type and attributes byte.
void setIDTEntry(uint8_t vector, void (*isr), uint16_t selector, uint8_t ist, uint8_t type_attr);

// PIC Stuff

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define ICW1_ICW4	0b1		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0b10		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0b100		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0b1000		/* Level triggered (edge) mode */
#define ICW1_INIT	0b10000		/* Initialization - required! */

#define ICW4_8086	0b1		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0b10		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0b1000		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0b1100		/* Buffered mode/master */
#define ICW4_SFNM	0b10000		/* Special fully nested (not) */

#define CASCADE_IRQ 2

void initPIC();
void remapPIC(int offset1, int offset2);
void disablePIC();
void setPICMask(uint8_t line);
void clearPICMask(uint8_t line);
void sendEOI(uint8_t irq);

// CPU instructions

/// @brief Output a byte to a port.
/// @param port The port to output to.
/// @param val The byte to output.
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

/// @brief Read a byte from a port.
/// @param port The port to read from.
/// @return The byte read.
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

static inline void outw(uint16_t port, uint16_t value) {
    __asm__("out %%ax, %%dx" : : "a"(value), "d"(port));
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm__ volatile ( "inw %w1, %w0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

static inline void outl(uint16_t port, uint32_t value) {
  __asm__ __volatile__("outl %%eax, %%dx" : : "d"(port), "a"(value));
}

static inline uint32_t inl(uint16_t port) {
  uint32_t ret;
  __asm__ __volatile__("inl %%dx, %%eax" : "=a"(ret) : "d"(port));
  return ret;
}

/// @brief Wait for an I/O operation to complete.
static inline void ioWait()
{
    outb(0x80, 0);
}

/// @brief Check if CPU interrupts are enabled.
/// @return true if interrupts are enabled, false otherwise.
static inline bool areInterruptsEnabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

static inline uint64_t getCR0(void) {
    uint64_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    return cr0;
}

static inline void setCR0(uint64_t cr0) {
    asm volatile ("mov %0, %%cr0" : : "r"(cr0));
}

static inline uint64_t getCR2(void) {
    uint64_t cr2;
    asm volatile ("mov %%cr2, %0" : "=r"(cr2));
    return cr2;
}

static inline uint64_t getCR3(void) {
    uint64_t cr3;
    asm volatile ("mov %%cr3, %0" : "=r"(cr3));
    return cr3;
}

static inline void setCR3(uint64_t cr3) {
    asm volatile ("mov %0, %%cr3" : : "r"(cr3));
}

static inline uint64_t getCR4(void) {
    uint64_t cr4;
    asm volatile ("mov %%cr4, %0" : "=r"(cr4));
    return cr4;
}

static inline void setCR4(uint64_t cr4) {
    asm volatile ("mov %0, %%cr4" : : "r"(cr4));
}

static inline void getCR8(uint64_t *cr8) {
    asm volatile ("mov %%cr8, %0" : "=r"(*cr8));
}

static inline void setCR8(uint64_t cr8) {
    asm volatile ("mov %0, %%cr8" : : "r"(cr8));
}

// Credits for CPUID code to elysium-os/cronus

#define X86_64_CPUID_DEFINE_FEATURE(LEAF, REGISTER, BIT) ((x86_64_cpuid_feature_t) { .leaf = (LEAF), .reg = (REGISTER), .bit = (BIT) })

#define X86_64_CPUID_FEATURE_SSE3 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 0)
#define X86_64_CPUID_FEATURE_PCLMUL X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 1)
#define X86_64_CPUID_FEATURE_DTES64 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 2)
#define X86_64_CPUID_FEATURE_MONITOR X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 3)
#define X86_64_CPUID_FEATURE_DS_CPL X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 4)
#define X86_64_CPUID_FEATURE_VMX X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 5)
#define X86_64_CPUID_FEATURE_SMX X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 6)
#define X86_64_CPUID_FEATURE_EST X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 7)
#define X86_64_CPUID_FEATURE_TM2 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 8)
#define X86_64_CPUID_FEATURE_SSSE3 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 9)
#define X86_64_CPUID_FEATURE_CID X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 10)
#define X86_64_CPUID_FEATURE_SDBG X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 11)
#define X86_64_CPUID_FEATURE_FMA X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 12)
#define X86_64_CPUID_FEATURE_CX16 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 13)
#define X86_64_CPUID_FEATURE_XTPR X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 14)
#define X86_64_CPUID_FEATURE_PDCM X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 15)
#define X86_64_CPUID_FEATURE_PCID X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 17)
#define X86_64_CPUID_FEATURE_DCA X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 18)
#define X86_64_CPUID_FEATURE_SSE4_1 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 19)
#define X86_64_CPUID_FEATURE_SSE4_2 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 20)
#define X86_64_CPUID_FEATURE_X2APIC X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 21)
#define X86_64_CPUID_FEATURE_MOVBE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 22)
#define X86_64_CPUID_FEATURE_POPCNT X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 23)
#define X86_64_CPUID_FEATURE_TSC_DEADLINE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 24)
#define X86_64_CPUID_FEATURE_AES X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 25)
#define X86_64_CPUID_FEATURE_XSAVE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 26)
#define X86_64_CPUID_FEATURE_OSXSAVE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 27)
#define X86_64_CPUID_FEATURE_AVX X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 28)
#define X86_64_CPUID_FEATURE_F16C X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 29)
#define X86_64_CPUID_FEATURE_RDRAND X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 30)
#define X86_64_CPUID_FEATURE_HYPERVISOR X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_ECX, 31)
#define X86_64_CPUID_FEATURE_FPU X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 0)
#define X86_64_CPUID_FEATURE_VME X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 1)
#define X86_64_CPUID_FEATURE_DE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 2)
#define X86_64_CPUID_FEATURE_PSE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 3)
#define X86_64_CPUID_FEATURE_TSC X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 4)
#define X86_64_CPUID_FEATURE_MSR X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 5)
#define X86_64_CPUID_FEATURE_PAE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 6)
#define X86_64_CPUID_FEATURE_MCE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 7)
#define X86_64_CPUID_FEATURE_CX8 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 8)
#define X86_64_CPUID_FEATURE_APIC X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 9)
#define X86_64_CPUID_FEATURE_SEP X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 11)
#define X86_64_CPUID_FEATURE_MTRR X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 12)
#define X86_64_CPUID_FEATURE_PGE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 13)
#define X86_64_CPUID_FEATURE_MCA X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 14)
#define X86_64_CPUID_FEATURE_CMOV X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 15)
#define X86_64_CPUID_FEATURE_PAT X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 16)
#define X86_64_CPUID_FEATURE_PSE36 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 17)
#define X86_64_CPUID_FEATURE_PSN X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 18)
#define X86_64_CPUID_FEATURE_CLFLUSH X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 19)
#define X86_64_CPUID_FEATURE_DS X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 21)
#define X86_64_CPUID_FEATURE_ACPI X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 22)
#define X86_64_CPUID_FEATURE_MMX X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 23)
#define X86_64_CPUID_FEATURE_FXSR X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 24)
#define X86_64_CPUID_FEATURE_SSE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 25)
#define X86_64_CPUID_FEATURE_SSE2 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 26)
#define X86_64_CPUID_FEATURE_SS X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 27)
#define X86_64_CPUID_FEATURE_HTT X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 28)
#define X86_64_CPUID_FEATURE_TM X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 29)
#define X86_64_CPUID_FEATURE_IA64 X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 30)
#define X86_64_CPUID_FEATURE_PBE X86_64_CPUID_DEFINE_FEATURE(1, X86_64_CPUID_REGISTER_EDX, 31)
#define X86_64_CPUID_FEATURE_ARAT X86_64_CPUID_DEFINE_FEATURE(6, X86_64_CPUID_REGISTER_EAX, 2)
#define X86_64_CPUID_FEATURE_AVX512 X86_64_CPUID_DEFINE_FEATURE(7, X86_64_CPUID_REGISTER_EBX, 16)
#define X86_64_CPUID_FEATURE_TSC_INVARIANT X86_64_CPUID_DEFINE_FEATURE(0x80000007, X86_64_CPUID_REGISTER_EDX, 8)

typedef enum {
    X86_64_CPUID_REGISTER_EAX,
    X86_64_CPUID_REGISTER_EBX,
    X86_64_CPUID_REGISTER_ECX,
    X86_64_CPUID_REGISTER_EDX
} x86_64_cpuid_register_t;

typedef struct {
    uint32_t leaf;
    x86_64_cpuid_register_t reg;
    uint32_t bit;
} x86_64_cpuid_feature_t;

static inline void cpuidRegister(uint32_t leaf, x86_64_cpuid_register_t reg, uint32_t *out) {
    uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;
    asm volatile("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(leaf), "c"(0));
    switch(reg) {
        case X86_64_CPUID_REGISTER_EAX: *out = eax; break;
        case X86_64_CPUID_REGISTER_EBX: *out = ebx; break;
        case X86_64_CPUID_REGISTER_ECX: *out = ecx; break;
        case X86_64_CPUID_REGISTER_EDX: *out = edx; break;
    }
}

static inline bool cpuidFeature(x86_64_cpuid_feature_t feature) {
    uint32_t value = 0;
    cpuidRegister(feature.leaf, feature.reg, &value);
    return (value & (1 << feature.bit));
}

// FPU Stuff

void initFPU();

static inline void xsave(void *area) {
    asm volatile("xsave (%0)" : : "r"(area), "a"(0xFFFFFFFF), "d"(0xFFFFFFFF) : "memory");
}

static inline void xrstor(void *area) {
    asm volatile("xrstor (%0)" : : "r"(area), "a"(0xFFFFFFFF), "d"(0xFFFFFFFF) : "memory");
}

static inline void fxsave(void *area) {
    asm volatile("fxsave (%0)" : : "r"(area) : "memory");
}

static inline void fxrstor(void *area) {
    asm volatile("fxrstor (%0)" : : "r"(area) : "memory");
}

// Timer stuff

#define TIMER_TIMEPERTICK 1000 // In Hz

void initTimer();
void timerTick();
uint64_t getTime(); // In milliseconds since boot
void sleep(uint64_t ms);

// PCI Stuff

typedef struct {
    uint16_t segment;
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
    int64_t listIndex;
} pci_device_t;

void initPCI();
void addPCIDevice(pci_device_t *device);
void removePCIDevice(pci_device_t *device);
uint8_t readPCIConfig8(pci_device_t *device, size_t offset);
uint16_t readPCIConfig16(pci_device_t *device, size_t offset);
uint32_t readPCIConfig32(pci_device_t *device, size_t offset);
void writePCIConfig8(pci_device_t *device, size_t offset, uint8_t value);
void writePCIConfig16(pci_device_t *device, size_t offset, uint16_t value);
void writePCIConfig32(pci_device_t *device, size_t offset, uint32_t value);

// ACPI STUFF

void initACPI(uint64_t rsdpAddress);

// Make the Stack Larger

static inline void initStack() {
    uint64_t 
}

#endif