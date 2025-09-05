/*
    The Kobold Kernel
    idt.c - Everything IDT-related, including ISRs and PIC
*/

#include <arch/x86_64/cpu.h>
#include <string.h>
#include <arch/x86_64/serial.h>
#include <arch/x86_64/printf.h>

/// @brief THe IDT Table
idt_entry_t idt[256];

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

/// @brief Load the IDT
void loadIDT() {
    idt_descriptor_t idt_desc = {
        .size = sizeof(idt) - 1,
        .offset = (uint64_t)&idt
    };
    asm volatile ("lidt %0" : : "m"(idt_desc));
}

/// @brief ISR 0 Defined in assembly
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();

void initIDT() {
    asm volatile ("cli"); // Disable interrupts during setup

    memset(&idt, 0, sizeof(idt)); // Clear the IDT

    setIDTEntry(0, (void*)isr0, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Divide by zero
    setIDTEntry(1, (void*)isr1, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Debug
    setIDTEntry(2, (void*)isr2, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Non Maskable Interrupt
    setIDTEntry(3, (void*)isr3, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_TRAP_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Breakpoint
    setIDTEntry(4, (void*)isr4, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_TRAP_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Overflow
    setIDTEntry(5, (void*)isr5, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Bound Range Exceeded
    setIDTEntry(6, (void*)isr6, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Invalid Opcode
    setIDTEntry(7, (void*)isr7, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Device Not Available
    setIDTEntry(8, (void*)isr8, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Double Fault
    setIDTEntry(9, (void*)isr9, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Coprocessor Segment Overrun
    setIDTEntry(10, (void*)isr10, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Invalid TSS
    setIDTEntry(11, (void*)isr11, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Segment Not Present
    setIDTEntry(12, (void*)isr12, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Stack-Segment Fault
    setIDTEntry(13, (void*)isr13, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // General Protection Fault
    setIDTEntry(14, (void*)isr14, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Page Fault
    setIDTEntry(16, (void*)isr16, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // x87 Floating-Point Exception
    setIDTEntry(17, (void*)isr17, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Alignment Check
    setIDTEntry(18, (void*)isr18, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Machine Check
    setIDTEntry(19, (void*)isr19, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // SIMD Floating-Point Exception
    setIDTEntry(20, (void*)isr20, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Virtualization Exception
    setIDTEntry(21, (void*)isr21, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Control Protection Exception
    setIDTEntry(28, (void*)isr28, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Hypervisor Injection Exception
    setIDTEntry(29, (void*)isr29, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // VMM Communication Exception
    setIDTEntry(30, (void*)isr30, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // Security Exception

    initPIC(); // Initialize and remap the PIC

    setIDTEntry(32, (void*)isr32, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ0 - Timer
    setIDTEntry(33, (void*)isr33, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ1 - Keyboard
    setIDTEntry(34, (void*)isr34, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ2 - Cascade (used internally by the two PICs. never raised)
    setIDTEntry(35, (void*)isr35, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ3 - COM2
    setIDTEntry(36, (void*)isr36, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ4 - COM1
    setIDTEntry(37, (void*)isr37, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ5 - LPT2
    setIDTEntry(38, (void*)isr38, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ6 - Floppy Disk
    setIDTEntry(39, (void*)isr39, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ7 - LPT1 / Spurious Interrupt
    setIDTEntry(40, (void*)isr40, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ8 - CMOS Real-Time Clock
    setIDTEntry(41, (void*)isr41, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ9 - Free for peripherals / legacy SCSI
    setIDTEntry(42, (void*)isr42, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ10 - Free for peripherals / SCSI / NIC
    setIDTEntry(43, (void*)isr43, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ11 - Free for peripherals / SCSI / NIC
    setIDTEntry(44, (void*)isr44, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ12 - PS2 Mouse
    setIDTEntry(45, (void*)isr45, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ13 - FPU / Coprocessor / Inter-processor
    setIDTEntry(46, (void*)isr46, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ14 - Primary ATA Hard Disk
    setIDTEntry(47, (void*)isr47, makeSegmentSelector(1, 0, 0), IDT_NO_IST, IDT_TYPE_INTERRUPT_GATE | IDT_TYPE_DPL0 | IDT_PRESENT); // IRQ15 - Secondary ATA Hard Disk

    clearPICMask(0);

    loadIDT();

    asm volatile ("sti"); // Re-enable interrupts after setup
}

/// @brief Handles crashes triggered by ISRs
/// @param regs The registers at the time of the interrupt
void crashISRHandler(isr_registers_t* regs) {
    printf("Crash! Interrupt: %d, Error: %d\n", regs->interrupt, regs->error);
    printf("Spilling registers:\n");
    printf("RAX: %016llx RBX: %016llx RCX: %016llx RDX: %016llx\n", regs->rax, regs->rbx, regs->rcx, regs->rdx);
    printf("RSI: %016llx RDI: %016llx RBP: %016llx\n", regs->rsi, regs->rdi, regs->rbp);
    printf("R8 : %016llx R9 : %016llx R10: %016llx R11: %016llx\n", regs->r8, regs->r9, regs->r10, regs->r11);
    printf("R12: %016llx R13: %016llx R14: %016llx R15: %016llx\n", regs->r12, regs->r13, regs->r14, regs->r15);
    printf("CS: %016x RIP: %016llx RFLAGS: %016llx\n", regs->cs, regs->rip, regs->rflags);
    printf("SS: %016x RSP: %016llx\n", regs->ss, regs->rsp);
    while (1) {
        asm volatile ("hlt");
    }
}

/// @brief A common ISR handler for all ISRs
/// @param regs The registers at the time of interrupt
void commonISRHandler(isr_registers_t* regs) {
    if (regs->interrupt == 0) { // Division by zero
        // Division by 0 should be considered a fatal bug and crash.
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 1) { // Debug
        printf("Debug Interrupt\n");
        // Ignore for now
    }
    else if (regs->interrupt == 2) {
        printf("Non Maskable Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 3) {
        printf("Breakpoint Interrupt\n");
        // Ignore for now
    }
    else if (regs->interrupt == 4) {
        printf("Overflow Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 5) {
        printf("Bound Range Exceeded Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 6) {
        printf("Invalid Opcode Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 7) {
        printf("Device Not Available Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 8) {
        printf("Double Fault Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 9) {
        printf("Coprocessor Segment Overrun Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 10) {
        printf("Invalid TSS Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 11) {
        printf("Segment Not Present Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 12) {
        printf("Stack-Segment Fault Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 13) {
        printf("General Protection Fault Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 14) {
        printf("Page Fault Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 16) {
        printf("x87 Floating-Point Exception Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 17) {
        printf("Alignment Check Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 18) {
        printf("Machine Check Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 19) {
        printf("SIMD Floating-Point Exception Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 20) {
        printf("Virtualization Exception Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 21) {
        printf("Control Protection Exception Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 28) {
        printf("Hypervisor Injection Exception Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 29) {
        printf("VMM Communication Exception Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 30) {
        printf("Security Exception Interrupt\n");
        crashISRHandler(regs);
    }
    else if (regs->interrupt == 32) {
        timerTick();
        sendEOI(0);
    }
    else if (regs->interrupt == 33) {
        sendEOI(1);
    }
    else if (regs->interrupt == 34) {
        sendEOI(2);
    }
    else if (regs->interrupt == 35) {
        sendEOI(3);
    }
    else if (regs->interrupt == 36) {
        sendEOI(4);
    }
    else if (regs->interrupt == 37) {
        sendEOI(5);
    }
    else if (regs->interrupt == 38) {
        sendEOI(6);
    }
    else if (regs->interrupt == 39) {
        sendEOI(7);
    }
    else if (regs->interrupt == 40) {
        sendEOI(8);
    }
    else if (regs->interrupt == 41) {
        sendEOI(9);
    }
    else if (regs->interrupt == 42) {
        sendEOI(10);
    }
    else if (regs->interrupt == 43) {
        sendEOI(11);
    }
    else if (regs->interrupt == 44) {
        sendEOI(12);
    }
    else if (regs->interrupt == 45) {
        sendEOI(13);
    }
    else if (regs->interrupt == 46) {
        sendEOI(14);
    }
    else if (regs->interrupt == 47) {
        sendEOI(15);
    }
    else {
        printf("Unhandled Interrupt: %d\n", regs->interrupt);
        crashISRHandler(regs);
    }
}

void initPIC() {
    outb(PIC1_DATA, 0xFF); // Fully mask interrupts on both PICs
    ioWait();
	outb(PIC2_DATA, 0xFF);
    ioWait();

    remapPIC(0x20, 0x28); // Remap PIC interrupts to 0x20-0x2F
}

void remapPIC(int offset1, int offset2) {
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);                        // save masks
    ioWait();
    a2 = inb(PIC2_DATA);
    ioWait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
    ioWait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    ioWait();
    outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
    ioWait();
    outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
    ioWait();
    outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    ioWait();
    outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
    ioWait();

    outb(PIC1_DATA, ICW4_8086);
    ioWait();
    outb(PIC2_DATA, ICW4_8086);
    ioWait();

    outb(PIC1_DATA, a1);   // restore saved masks.
    ioWait();
    outb(PIC2_DATA, a2);
    ioWait();
}

void disablePIC() {
    outb(PIC1_DATA, 0xFF); // Fully mask interrupts on both PICs
    ioWait();
    outb(PIC2_DATA, 0xFF);
    ioWait();
}

void setPICMask(uint8_t line) {
    uint16_t port;
    uint8_t value;

    if(line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        line -= 8;
    }
    value = inb(port) | (1 << line);
    outb(port, value);   
}

void clearPICMask(uint8_t line) {
    uint16_t port;
    uint8_t value;

    if(line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        line -= 8;
    }
    value = inb(port) & ~(1 << line);
    outb(port, value);   
}

void sendEOI(uint8_t irq) {
    if(irq >= 8) {
        outb(PIC2_COMMAND, 0x20); // Send reset signal to slave.
    }
    outb(PIC1_COMMAND, 0x20); // Send reset signal to master. 
}