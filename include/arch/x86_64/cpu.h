#ifndef X86_64_CPU_H
#define X86_64_CPU_H

#include <stdint.h>
#include <stdbool.h>

/*
    The Kobold Kernel
    cpu.h - Pretty much everything CPU related for x86_64.
    Includes:
        GDT
        IDT
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

typedef struct {
    uint16_t limit0; // Limit 0:15
    uint16_t base0; // Base 0:15
    uint8_t base1; // Base 16:23
    uint8_t access; // Access Byte
    uint8_t limit1_flags; // Limit 16:19 and flags
    uint8_t base2; // Base 24:31
} __attribute__((packed)) gdt_entry_t;

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

typedef struct {
    gdt_entry_t null;
    gdt_entry_t kernel_code;
    gdt_entry_t kernel_data;
    gdt_entry_t user_code;
    gdt_entry_t user_data;
    tss_descriptor_entry_t tss_desc;
} __attribute__((packed)) gdt_t;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_pointer_t;

#define makeSegmentSelector(index, table, rpl) (uint16_t)(((uint16_t)index << 3) | ((bool)(table & 0b1) << 2) | ((uint8_t)(rpl & 0b11)))

void initGDT();
void reloadGDT();

// IDT Stuff

typedef struct {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) idt_descriptor_t;

typedef struct {
    uint16_t offset0; // Offset 0:15
    uint16_t selector; // A code segment selector in GDT or LDT
    uint8_t ist; // Bits 0-2 hold Interrupt Stack Table offset, rest of bits zero.
    uint8_t type_attr; // Type and attributes
    uint16_t offset1; // Offset 16:31
    uint32_t offset2; // Offset 32:63
    uint32_t reserved; // Reserved
} __attribute__((packed)) idt_entry_t;



#endif