#include <arch/x86_64/cpu.h>
#include <arch/x86_64/serial.h>
#include <arch/x86_64/printf.h>

idt_entry_t idt[256] = {0};

void setIDTEntry(uint8_t vector, void (*isr), uint16_t selector, uint8_t ist, uint8_t type_attr) {
    uint64_t offset = (uint64_t)isr;
    idt[vector] = (idt_entry_t){
        .offset0 = offset & 0xFFFF,
        .selector = selector,
        .ist = ist & 0b111,
        .type_attr = type_attr,
        .offset1 = (uint16_t)((offset >> 16) & 0xFFFF),
        .offset2 = (uint32_t)((offset >> 32) & 0xFFFFFFFF),
        .reserved = 0
    };
}

void loadIDT() {
    idt_descriptor_t idt_desc = {
        .size = sizeof(idt) - 1,
        .offset = (uint64_t)&idt
    };
    asm volatile ("lidt %0" : : "m"(idt_desc));
}

extern void isr0();

void initIDT() {
    asm volatile ("cli"); // Disable interrupts during setup

    setIDTEntry(0, (void*)isr0, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Divide by zero

    loadIDT();

    asm volatile ("sti"); // Re-enable interrupts after setup
}

void crashISRHandler(isr_registers_t* regs) {
    printf("Crash! Interrupt: %d, Error: %d\n", regs->interrupt, regs->error);
    printf("Spilling registers:\n");
    printf("RAX: %016x RBX: %016x RCX: %016x RDX: %016x\n", regs->rax, regs->rbx, regs->rcx, regs->rdx);
    printf("RSI: %016x RDI: %016x RBP: %016x\n", regs->rsi, regs->rdi, regs->rbp);
    printf("R8 : %016x R9 : %016x R10: %016x R11: %016x\n", regs->r8, regs->r9, regs->r10, regs->r11);
    printf("R12: %016x R13: %016x R14: %016x R15: %016x\n", regs->r12, regs->r13, regs->r14, regs->r15);
    printf("CS: %016x RIP: %016x RFLAGS: %016x\n", regs->cs, regs->rip, regs->rflags);
    printf("SS: %016x RSP: %016x\n", regs->ss, regs->rsp);
    while (1) {
        asm volatile ("hlt");
    }
}

void commonISRHandler(isr_registers_t* regs) {
    if (regs->interrupt < 32) {
        crashISRHandler(regs);
    } else {
        printf("Interrupt: %d\n", regs->interrupt);
    }
}