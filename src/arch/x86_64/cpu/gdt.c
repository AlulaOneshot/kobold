#include <arch/x86_64/cpu.h>

gdt_t gdt;
tss_t tss;

void setTSS(tss_t *tss) {
    uint64_t base = (uint64_t)tss;
    uint32_t limit = sizeof(tss_t) - 1;

    gdt.tss_desc = (tss_descriptor_entry_t){
        .limit0 = limit & 0xFFFF,
        .base0 = base & 0xFFFF,
        .base1 = (base >> 16) & 0xFF,
        .access = ACCESS_PRESENT | ACCESS_PRIVILEGE_RING0 | 0x09, // 0x09 = Available 64-bit TSS
        .limit1_flags = (limit >> 16) & 0x0F,
        .base2 = (base >> 24) & 0xFF,
        .base3 = (base >> 32) & 0xFFFFFFFF,
        .reserved = 0
    };
}

extern void loadCS(uint16_t segment);

void reloadGDT() {
    gdt_pointer_t gdt_ptr = {
        .limit = sizeof(gdt) - 1,
        .base = (uint64_t)&gdt
    };

    asm volatile ("lgdt %0" : : "m"(gdt_ptr)); // LGDT expects a pointer to the GDT pointer.

    loadCS(makeSegmentSelector(1, 0, 0));

    uint16_t data_sel = makeSegmentSelector(2, 0, 0);
    asm volatile("mov %0, %%ds" : : "r"(data_sel));
    asm volatile("mov %0, %%es" : : "r"(data_sel));
    asm volatile("mov %0, %%ss" : : "r"(data_sel));
    asm volatile("mov %0, %%fs" : : "r"(data_sel));
    asm volatile("mov %0, %%gs" : : "r"(data_sel));
}

void initGDT() {
    gdt.null = (gdt_entry_t){0};
    gdt.kernel_code = (gdt_entry_t){
        .limit0 = 0xFFFF,
        .base0 = 0x0000,
        .base1 = 0x00,
        .access = ACCESS_PRESENT | ACCESS_PRIVILEGE_RING0 | ACCESS_DESCRIPTOR_TYPE | ACCESS_EXECUTABLE | ACCESS_READ_WRITE,
        .limit1_flags = (FLAGS_LONG_MODE_CODE | FLAGS_GRANULARITY) | 0x0F,
        .base2 = 0x00
    };
    gdt.kernel_data = (gdt_entry_t){
        .limit0 = 0xFFFF,
        .base0 = 0x0000,
        .base1 = 0x00,
        .access = ACCESS_PRESENT | ACCESS_PRIVILEGE_RING0 | ACCESS_DESCRIPTOR_TYPE | ACCESS_READ_WRITE,
        .limit1_flags = FLAGS_GRANULARITY | 0x0F,
        .base2 = 0x00
    };
    gdt.user_code = (gdt_entry_t){
        .limit0 = 0xFFFF,
        .base0 = 0x0000,
        .base1 = 0x00,
        .access = ACCESS_PRESENT | ACCESS_PRIVILEGE_RING3 | ACCESS_DESCRIPTOR_TYPE | ACCESS_EXECUTABLE | ACCESS_READ_WRITE,
        .limit1_flags = (FLAGS_LONG_MODE_CODE | FLAGS_GRANULARITY) | 0x0F,
        .base2 = 0x00
    };
    gdt.user_data = (gdt_entry_t){
        .limit0 = 0xFFFF,
        .base0 = 0x0000,
        .base1 = 0x00,
        .access = ACCESS_PRESENT | ACCESS_PRIVILEGE_RING3 | ACCESS_DESCRIPTOR_TYPE | ACCESS_READ_WRITE,
        .limit1_flags = FLAGS_GRANULARITY | 0x0F,
        .base2 = 0x00
    };

    tss = (tss_t){
        .reserved0 = 0,
        .rsp0 = 0,
        .rsp1 = 0,
        .rsp2 = 0,
        .reserved1 = 0,
        .ist1 = 0,
        .ist2 = 0,
        .ist3 = 0,
        .ist4 = 0,
        .ist5 = 0,
        .ist6 = 0,
        .ist7 = 0,
        .reserved2 = 0,
        .reserved3 = 0,
        .io_map_base = 0
    };

    setTSS(&tss);
    
    reloadGDT();
}