#include <arch/x86_64/cpu.h>
#include <arch/x86_64/printf.h>

idt_entry_t idt[256];

isr_handler_t isr_handlers[256] = {0};

void setIDTGate(uint8_t vector, uint64_t offset, uint16_t segment, uint8_t ist, uint8_t flags) {
    idt[vector].offset0 = (offset & 0xFFFF);
    idt[vector].offset1 = (offset >> 16) & 0xFFFF;
    idt[vector].offset2 = (offset >> 32) & 0xFFFFFFFF;

    idt[vector].ist_reserved = ist & 0b111;
    idt[vector].flags = flags & 0b11101111;
    idt[vector].segmentSelector = segment;
}

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
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
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
extern void isr48();
extern void isr49();
extern void isr50();
extern void isr51();
extern void isr52();
extern void isr53();
extern void isr54();
extern void isr55();
extern void isr56();
extern void isr57();
extern void isr58();
extern void isr59();
extern void isr60();
extern void isr61();
extern void isr62();
extern void isr63();
extern void isr64();
extern void isr65();
extern void isr66();
extern void isr67();
extern void isr68();
extern void isr69();
extern void isr70();
extern void isr71();
extern void isr72();
extern void isr73();
extern void isr74();
extern void isr75();
extern void isr76();
extern void isr77();
extern void isr78();
extern void isr79();
extern void isr80();
extern void isr81();
extern void isr82();
extern void isr83();
extern void isr84();
extern void isr85();
extern void isr86();
extern void isr87();
extern void isr88();
extern void isr89();
extern void isr90();
extern void isr91();
extern void isr92();
extern void isr93();
extern void isr94();
extern void isr95();
extern void isr96();
extern void isr97();
extern void isr98();
extern void isr99();
extern void isr100();
extern void isr101();
extern void isr102();
extern void isr103();
extern void isr104();
extern void isr105();
extern void isr106();
extern void isr107();
extern void isr108();
extern void isr109();
extern void isr110();
extern void isr111();
extern void isr112();
extern void isr113();
extern void isr114();
extern void isr115();
extern void isr116();
extern void isr117();
extern void isr118();
extern void isr119();
extern void isr120();
extern void isr121();
extern void isr122();
extern void isr123();
extern void isr124();
extern void isr125();
extern void isr126();
extern void isr127();
extern void isr128();
extern void isr129();
extern void isr130();
extern void isr131();
extern void isr132();
extern void isr133();
extern void isr134();
extern void isr135();
extern void isr136();
extern void isr137();
extern void isr138();
extern void isr139();
extern void isr140();
extern void isr141();
extern void isr142();
extern void isr143();
extern void isr144();
extern void isr145();
extern void isr146();
extern void isr147();
extern void isr148();
extern void isr149();
extern void isr150();
extern void isr151();
extern void isr152();
extern void isr153();
extern void isr154();
extern void isr155();
extern void isr156();
extern void isr157();
extern void isr158();
extern void isr159();
extern void isr160();
extern void isr161();
extern void isr162();
extern void isr163();
extern void isr164();
extern void isr165();
extern void isr166();
extern void isr167();
extern void isr168();
extern void isr169();
extern void isr170();
extern void isr171();
extern void isr172();
extern void isr173();
extern void isr174();
extern void isr175();
extern void isr176();
extern void isr177();
extern void isr178();
extern void isr179();
extern void isr180();
extern void isr181();
extern void isr182();
extern void isr183();
extern void isr184();
extern void isr185();
extern void isr186();
extern void isr187();
extern void isr188();
extern void isr189();
extern void isr190();
extern void isr191();
extern void isr192();
extern void isr193();
extern void isr194();
extern void isr195();
extern void isr196();
extern void isr197();
extern void isr198();
extern void isr199();
extern void isr200();
extern void isr201();
extern void isr202();
extern void isr203();
extern void isr204();
extern void isr205();
extern void isr206();
extern void isr207();
extern void isr208();
extern void isr209();
extern void isr210();
extern void isr211();
extern void isr212();
extern void isr213();
extern void isr214();
extern void isr215();
extern void isr216();
extern void isr217();
extern void isr218();
extern void isr219();
extern void isr220();
extern void isr221();
extern void isr222();
extern void isr223();
extern void isr224();
extern void isr225();
extern void isr226();
extern void isr227();
extern void isr228();
extern void isr229();
extern void isr230();
extern void isr231();
extern void isr232();
extern void isr233();
extern void isr234();
extern void isr235();
extern void isr236();
extern void isr237();
extern void isr238();
extern void isr239();
extern void isr240();
extern void isr241();
extern void isr242();
extern void isr243();
extern void isr244();
extern void isr245();
extern void isr246();
extern void isr247();
extern void isr248();
extern void isr249();
extern void isr250();
extern void isr251();
extern void isr252();
extern void isr253();
extern void isr254();
extern void isr255();

typedef struct debugStackFrame {
    struct debugStackFrame *rbp;
    uint64_t rip;
} debugStackFrame_t;

void printStackTrace(debugStackFrame_t *initialFrame) {
    printf("Stack Trace:\n");
    for (uint64_t i = 0; initialFrame != NULL && initialFrame->rip != 0 && i < 30; i++) {
        printf("  [%d] RIP: 0x%016llx\n", i, initialFrame->rip);
        initialFrame = initialFrame->rbp;
    }
}

void exceptionUnhandled(isr_registers_t* regs) {
    printf("Unhandled Exception: (Interrupt %d) On CPU %d\n", regs->interrupt, currentCPU);

    debugStackFrame_t initialStackFrame;
    initialStackFrame.rbp = (debugStackFrame_t *)regs->rbp;
    initialStackFrame.rip = regs->rip;
    printStackTrace(&initialStackFrame);

    if (regs->interrupt == 14) {
        uint64_t cr2;
        asm volatile ("mov %%cr2, %0" : "=r"(cr2));
        printf("Faulting Address: 0x%llx\n", cr2);
    }

    printf("Interrupt Frame:\n  Interrupt Number: %d\n  Error Code: 0x%llx\n  RIP: 0x%llx\n  CS: 0x%llx\n  RFLAGS: 0x%llx\n  RSP: 0x%llx\n  SS: 0x%llx\n", regs->interrupt, regs->error, regs->rip, regs->cs, regs->rflags, regs->rsp, regs->ss);

    printf("General Purpose Registers:\n  R15: 0x%llx\n  R14: 0x%llx\n  R13: 0x%llx\n  R12: 0x%llx\n  R11: 0x%llx\n  R10: 0x%llx\n  R9: 0x%llx\n  R8: 0x%llx\n  RDI: 0x%llx\n  RSI: 0x%llx\n  RBP: 0x%llx\n  RDX: 0x%llx\n  RCX: 0x%llx\n  RBX: 0x%llx\n  RAX: 0x%llx\n", regs->r15, regs->r14, regs->r13, regs->r12, regs->r11, regs->r10, regs->r9, regs->r8, regs->rdi, regs->rsi, regs->rbp, regs->rdx, regs->rcx, regs->rbx, regs->rax);

    asm volatile ("cli");
    while (1) {
        asm volatile ("hlt");
    }
}

/// @brief Sets up and Loads the IDT
void initialiseIDT() {
    setIDTGate(0, (uint64_t)isr0, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Divide by zero
    setIDTGate(1, (uint64_t)isr1, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Debug
    setIDTGate(2, (uint64_t)isr2, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Non Maskable Interrupt
    setIDTGate(3, (uint64_t)isr3, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_TRAP | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Breakpoint
    setIDTGate(4, (uint64_t)isr4, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_TRAP | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Overflow
    setIDTGate(5, (uint64_t)isr5, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Bound Range Exceeded
    setIDTGate(6, (uint64_t)isr6, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Invalid Opcode
    setIDTGate(7, (uint64_t)isr7, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Device Not Available
    setIDTGate(8, (uint64_t)isr8, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Double Fault
    setIDTGate(9, (uint64_t)isr9, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Coprocessor Segment Overrun
    setIDTGate(10, (uint64_t)isr10, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Invalid TSS
    setIDTGate(11, (uint64_t)isr11, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Segment Not Present
    setIDTGate(12, (uint64_t)isr12, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Stack-Segment Fault
    setIDTGate(13, (uint64_t)isr13, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // General Protection Fault
    setIDTGate(14, (uint64_t)isr14, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Page Fault
    setIDTGate(15, (uint64_t)isr15, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Reserved
    setIDTGate(16, (uint64_t)isr16, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // x87 Floating-Point Exception
    setIDTGate(17, (uint64_t)isr17, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Alignment Check
    setIDTGate(18, (uint64_t)isr18, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Machine Check
    setIDTGate(19, (uint64_t)isr19, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // SIMD Floating-Point Exception
    setIDTGate(20, (uint64_t)isr20, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Virtualization Exception
    setIDTGate(21, (uint64_t)isr21, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Control Protection Exception
    setIDTGate(22, (uint64_t)isr22, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Reserved
    setIDTGate(23, (uint64_t)isr23, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Reserved
    setIDTGate(24, (uint64_t)isr24, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Reserved
    setIDTGate(25, (uint64_t)isr25, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Reserved
    setIDTGate(26, (uint64_t)isr26, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Reserved
    setIDTGate(27, (uint64_t)isr27, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Reserved
    setIDTGate(28, (uint64_t)isr28, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Hypervisor Injection Exception
    setIDTGate(29, (uint64_t)isr29, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // VMM Communication Exception
    setIDTGate(30, (uint64_t)isr30, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // Security Exception
    setIDTGate(31, (uint64_t)isr31, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_3 | IDT_FLAG_PRESENT); // Syscall
    
    // IRQs
    // TODO: Remap PIC

    setIDTGate(32, (uint64_t)isr32, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ0 - Timer
    setIDTGate(33, (uint64_t)isr33, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ1 - Keyboard
    setIDTGate(34, (uint64_t)isr34, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ2 - Cascade (used internally by the two PICs. never raised)
    setIDTGate(35, (uint64_t)isr35, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ3 - COM2
    setIDTGate(36, (uint64_t)isr36, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ4 - COM1
    setIDTGate(37, (uint64_t)isr37, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ5 - LPT2
    setIDTGate(38, (uint64_t)isr38, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ6 - Floppy Disk
    setIDTGate(39, (uint64_t)isr39, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ7 - LPT1 / Spurious Interrupt
    setIDTGate(40, (uint64_t)isr40, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ8 - CMOS Real-Time Clock
    setIDTGate(41, (uint64_t)isr41, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ9 - Free for peripherals / legacy SCSI
    setIDTGate(42, (uint64_t)isr42, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ10 - Free for peripherals / SCSI / NIC
    setIDTGate(43, (uint64_t)isr43, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ11 - Free for peripherals / SCSI / NIC
    setIDTGate(44, (uint64_t)isr44, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ12 - PS2 Mouse
    setIDTGate(45, (uint64_t)isr45, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ13 - FPU / Coprocessor / Inter-processor
    setIDTGate(46, (uint64_t)isr46, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ14 - Primary ATA Hard Disk
    setIDTGate(47, (uint64_t)isr47, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT); // IRQ15 - Secondary ATA Hard Disk

    setIDTGate(48, (uint64_t)isr48, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(49, (uint64_t)isr49, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(50, (uint64_t)isr50, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(51, (uint64_t)isr51, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(52, (uint64_t)isr52, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(53, (uint64_t)isr53, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(54, (uint64_t)isr54, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(55, (uint64_t)isr55, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(56, (uint64_t)isr56, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(57, (uint64_t)isr57, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(58, (uint64_t)isr58, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(59, (uint64_t)isr59, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(60, (uint64_t)isr60, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(61, (uint64_t)isr61, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(62, (uint64_t)isr62, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(63, (uint64_t)isr63, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(64, (uint64_t)isr64, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(65, (uint64_t)isr65, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(66, (uint64_t)isr66, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(67, (uint64_t)isr67, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(68, (uint64_t)isr68, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(69, (uint64_t)isr69, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(70, (uint64_t)isr70, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(71, (uint64_t)isr71, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(72, (uint64_t)isr72, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(73, (uint64_t)isr73, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(74, (uint64_t)isr74, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(75, (uint64_t)isr75, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(76, (uint64_t)isr76, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(77, (uint64_t)isr77, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(78, (uint64_t)isr78, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(79, (uint64_t)isr79, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(80, (uint64_t)isr80, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(81, (uint64_t)isr81, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(82, (uint64_t)isr82, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(83, (uint64_t)isr83, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(84, (uint64_t)isr84, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(85, (uint64_t)isr85, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(86, (uint64_t)isr86, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(87, (uint64_t)isr87, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(88, (uint64_t)isr88, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(89, (uint64_t)isr89, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(90, (uint64_t)isr90, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(91, (uint64_t)isr91, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(92, (uint64_t)isr92, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(93, (uint64_t)isr93, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(94, (uint64_t)isr94, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(95, (uint64_t)isr95, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(96, (uint64_t)isr96, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(97, (uint64_t)isr97, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(98, (uint64_t)isr98, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(99, (uint64_t)isr99, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(100, (uint64_t)isr100, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(101, (uint64_t)isr101, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(102, (uint64_t)isr102, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(103, (uint64_t)isr103, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(104, (uint64_t)isr104, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(105, (uint64_t)isr105, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(106, (uint64_t)isr106, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(107, (uint64_t)isr107, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(108, (uint64_t)isr108, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(109, (uint64_t)isr109, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(110, (uint64_t)isr110, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(111, (uint64_t)isr111, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(112, (uint64_t)isr112, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(113, (uint64_t)isr113, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(114, (uint64_t)isr114, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(115, (uint64_t)isr115, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(116, (uint64_t)isr116, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(117, (uint64_t)isr117, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(118, (uint64_t)isr118, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(119, (uint64_t)isr119, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(120, (uint64_t)isr120, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(121, (uint64_t)isr121, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(122, (uint64_t)isr122, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(123, (uint64_t)isr123, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(124, (uint64_t)isr124, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(125, (uint64_t)isr125, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(126, (uint64_t)isr126, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(127, (uint64_t)isr127, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(128, (uint64_t)isr128, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_3 | IDT_FLAG_PRESENT);
    setIDTGate(129, (uint64_t)isr129, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(130, (uint64_t)isr130, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(131, (uint64_t)isr131, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(132, (uint64_t)isr132, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(133, (uint64_t)isr133, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(134, (uint64_t)isr134, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(135, (uint64_t)isr135, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(136, (uint64_t)isr136, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(137, (uint64_t)isr137, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(138, (uint64_t)isr138, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(139, (uint64_t)isr139, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(140, (uint64_t)isr140, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(141, (uint64_t)isr141, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(142, (uint64_t)isr142, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(143, (uint64_t)isr143, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(144, (uint64_t)isr144, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(145, (uint64_t)isr145, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(146, (uint64_t)isr146, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(147, (uint64_t)isr147, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(148, (uint64_t)isr148, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(149, (uint64_t)isr149, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(150, (uint64_t)isr150, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(151, (uint64_t)isr151, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(152, (uint64_t)isr152, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(153, (uint64_t)isr153, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(154, (uint64_t)isr154, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(155, (uint64_t)isr155, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(156, (uint64_t)isr156, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(157, (uint64_t)isr157, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(158, (uint64_t)isr158, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(159, (uint64_t)isr159, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(160, (uint64_t)isr160, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(161, (uint64_t)isr161, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(162, (uint64_t)isr162, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(163, (uint64_t)isr163, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(164, (uint64_t)isr164, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(165, (uint64_t)isr165, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(166, (uint64_t)isr166, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(167, (uint64_t)isr167, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(168, (uint64_t)isr168, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(169, (uint64_t)isr169, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(170, (uint64_t)isr170, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(171, (uint64_t)isr171, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(172, (uint64_t)isr172, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(173, (uint64_t)isr173, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(174, (uint64_t)isr174, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(175, (uint64_t)isr175, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(176, (uint64_t)isr176, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(177, (uint64_t)isr177, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(178, (uint64_t)isr178, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(179, (uint64_t)isr179, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(180, (uint64_t)isr180, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(181, (uint64_t)isr181, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(182, (uint64_t)isr182, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(183, (uint64_t)isr183, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(184, (uint64_t)isr184, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(185, (uint64_t)isr185, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(186, (uint64_t)isr186, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(187, (uint64_t)isr187, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(188, (uint64_t)isr188, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(189, (uint64_t)isr189, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(190, (uint64_t)isr190, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(191, (uint64_t)isr191, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(192, (uint64_t)isr192, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(193, (uint64_t)isr193, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(194, (uint64_t)isr194, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(195, (uint64_t)isr195, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(196, (uint64_t)isr196, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(197, (uint64_t)isr197, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(198, (uint64_t)isr198, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(199, (uint64_t)isr199, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(200, (uint64_t)isr200, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(201, (uint64_t)isr201, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(202, (uint64_t)isr202, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(203, (uint64_t)isr203, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(204, (uint64_t)isr204, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(205, (uint64_t)isr205, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(206, (uint64_t)isr206, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(207, (uint64_t)isr207, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(208, (uint64_t)isr208, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(209, (uint64_t)isr209, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(210, (uint64_t)isr210, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(211, (uint64_t)isr211, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(212, (uint64_t)isr212, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(213, (uint64_t)isr213, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(214, (uint64_t)isr214, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(215, (uint64_t)isr215, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(216, (uint64_t)isr216, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(217, (uint64_t)isr217, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(218, (uint64_t)isr218, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(219, (uint64_t)isr219, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(220, (uint64_t)isr220, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(221, (uint64_t)isr221, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(222, (uint64_t)isr222, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(223, (uint64_t)isr223, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(224, (uint64_t)isr224, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(225, (uint64_t)isr225, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(226, (uint64_t)isr226, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(227, (uint64_t)isr227, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(228, (uint64_t)isr228, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_3 | IDT_FLAG_PRESENT);
    setIDTGate(229, (uint64_t)isr229, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(230, (uint64_t)isr230, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(231, (uint64_t)isr231, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(232, (uint64_t)isr232, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(233, (uint64_t)isr233, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(234, (uint64_t)isr234, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(235, (uint64_t)isr235, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(236, (uint64_t)isr236, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(237, (uint64_t)isr237, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(238, (uint64_t)isr238, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(239, (uint64_t)isr239, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(240, (uint64_t)isr240, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(241, (uint64_t)isr241, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(242, (uint64_t)isr242, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(243, (uint64_t)isr243, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(244, (uint64_t)isr244, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(245, (uint64_t)isr245, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(246, (uint64_t)isr246, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(247, (uint64_t)isr247, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(248, (uint64_t)isr248, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(249, (uint64_t)isr249, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(250, (uint64_t)isr250, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(251, (uint64_t)isr251, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(252, (uint64_t)isr252, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(253, (uint64_t)isr253, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(254, (uint64_t)isr254, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);
    setIDTGate(255, (uint64_t)isr255, makeSegmentSelector(1, 0, 0), IDT_IST_NO_IST, IDT_FLAG_TYPE_INTERRUPT | IDT_FLAG_DPL_0 | IDT_FLAG_PRESENT);

    for (int i = 0; i < 256; i++) {
        isr_handlers[i].handler = exceptionUnhandled;
    }

    idt_descriptor_t desc = {
        .offset = (uint64_t)&idt[0],
        .size = sizeof(idt_entry_t) * 256
    };
    asm volatile ("lidt %0" : : "m"(desc));
}

void commonISRHandler(isr_registers_t* regs) {
    isr_handlers[regs->interrupt].handler(regs);
}