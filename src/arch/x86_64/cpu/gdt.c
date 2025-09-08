#include <arch/x86_64/cpu.h>

gdt_t gdt = {0};

extern void loadCS(uint16_t segment);

void initialiseGDT() {
    gdt.null = (gdt_entry_t){
        .access = 0,
        .base0 = 0,
        .base1 = 0,
        .base2 = 0,
        .limit0 = 0,
        .limit1_flags = 0
    };
    gdt.kernel_code = (gdt_entry_t){
        .access = GDT_ACCESS_ACCESSED | GDT_ACCESS_READWRITE | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_DESCRIPTOR_TYPE | IDT_FLAG_DPL_0 | GDT_ACCESS_PRESENT,
        .base0 = 0,
        .base1 = 0,
        .base2 = 0,
        .limit0 = 0xFFFF,
        .limit1_flags = 0xF | GDT_FLAGS_LONG_MODE_CODE | GDT_FLAGS_GRANULARITY
    };
    gdt.kernel_data = (gdt_entry_t){
        .access = GDT_ACCESS_ACCESSED | GDT_ACCESS_READWRITE | GDT_ACCESS_DESCRIPTOR_TYPE | IDT_FLAG_DPL_0 | GDT_ACCESS_PRESENT,
        .base0 = 0,
        .base1 = 0,
        .base2 = 0,
        .limit0 = 0xFFFF,
        .limit1_flags = 0xF | GDT_FLAGS_GRANULARITY | GDT_FLAGS_SIZE
    };
    gdt.user_code = (gdt_entry_t){
        .access = GDT_ACCESS_ACCESSED | GDT_ACCESS_READWRITE | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_DESCRIPTOR_TYPE | IDT_FLAG_DPL_3 | GDT_ACCESS_PRESENT,
        .base0 = 0,
        .base1 = 0,
        .base2 = 0,
        .limit0 = 0xFFFF,
        .limit1_flags = 0xF | GDT_FLAGS_LONG_MODE_CODE | GDT_FLAGS_GRANULARITY
    };
    gdt.user_data = (gdt_entry_t){
        .access = GDT_ACCESS_ACCESSED | GDT_ACCESS_READWRITE | GDT_ACCESS_DESCRIPTOR_TYPE | IDT_FLAG_DPL_3 | GDT_ACCESS_PRESENT,
        .base0 = 0,
        .base1 = 0,
        .base2 = 0,
        .limit0 = 0xFFFF,
        .limit1_flags = 0xF | GDT_FLAGS_GRANULARITY | GDT_FLAGS_SIZE
    };

    gdt_descriptor_t descriptor = {
        .base = (uint64_t)&gdt,
        .limit = sizeof(gdt_t) - 1
    };

    asm volatile ("lgdt %0" : : "m"(descriptor));

    loadCS(makeSegmentSelector(1, 0, 0));

    uint16_t data_sel = makeSegmentSelector(2, 0, 0);
    asm volatile("mov %0, %%ds" : : "r"(data_sel));
    asm volatile("mov %0, %%es" : : "r"(data_sel));
    asm volatile("mov %0, %%ss" : : "r"(data_sel));
    asm volatile("mov %0, %%fs" : : "r"(data_sel));
    asm volatile("mov %0, %%gs" : : "r"(data_sel));
}