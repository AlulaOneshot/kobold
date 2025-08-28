#include <cpu.h>
#include <log.h>

idt_entry_t idt[256] = {0};

void setGateIDT(uint16_t gate, uint64_t base, uint16_t selector, uint8_t flags) {
    idt[gate].flags = flags;
    idt[gate].selector = selector;
    idt[gate].base0 = base & 0xFFFF;
    idt[gate].base1 = (base >> 16) & 0xFFFF;
    idt[gate].base2 = (base >> 32) & 0xFFFFFFFF;
}

void initIDT() {
    asm volatile("cli"); // Disable Interrupts

    // Effectively disable PIC1 and PIC2 respectively by masking all interrupts
    outb(0x0021, 0xff);
    outb(0x00A1, 0xff);

    koboldLogLn("[Kobold/CPU/IDT] Initializing IDT...");

    // TODO: Set IDT Gates

    // TODO: Remap PIC

    lidt(idt, sizeof(idt)); // Load The IDT
    asm volatile("sti"); // Enable Interrupts

    koboldLogLn("[Kobold/CPU/IDT] IDT Initialized");
}

void isrCommonC() {

}