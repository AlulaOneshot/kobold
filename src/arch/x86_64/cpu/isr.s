# The Kobold Kernel
# isr.s - Automatically generate ISRs from macro

.section .text
.code64

.extern commonISRHandler

.macro isr_stub num
    .globl isr\num
    isr\num:
        pushq   $0
        pushq   $\num
        jmp isrCommonASM
.endm

.macro isr_stub_err num
    .globl  isr\num
    isr\num:
        pushq   $\num
        jmp     isrCommonASM
.endm

isrCommonASM:
    cli
        
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    mov %ds, %rbp
    push %rbp

    mov %rsp, %rdi
    
    call commonISRHandler

    pop %rbp
    mov %rbp, %ds

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rbp
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax

    add $16, %rsp

    sti
    iretq

isr_stub 0
isr_stub 1
isr_stub 2
isr_stub 3
isr_stub 4
isr_stub 5
isr_stub 6
isr_stub 7
isr_stub_err 8
isr_stub 9
isr_stub_err 10
isr_stub_err 11
isr_stub_err 12
isr_stub_err 13
isr_stub_err 14
isr_stub 16
isr_stub_err 17
isr_stub 18
isr_stub 19
isr_stub 20
isr_stub_err 21
isr_stub 28
isr_stub_err 29
isr_stub_err 30
isr_stub 32 # IRQ0 - Timer
isr_stub 33 # IRQ1 - Keyboard
isr_stub 34 # IRQ2 - Cascade (used internally by the two PICs. never raised)
isr_stub 35 # IRQ3 - COM2
isr_stub 36 # IRQ4 - COM1
isr_stub 37 # IRQ5 - LPT2
isr_stub 38 # IRQ6 - Floppy Disk
isr_stub 39 # IRQ7 - LPT1 / Spurious Interrupt
isr_stub 40 # IRQ8 - CMOS Real-Time Clock
isr_stub 41 # IRQ9 - Free for peripherals / legacy SCSI / NIC
isr_stub 42 # IRQ10 - Free for peripherals / SCSI / NIC
isr_stub 43 # IRQ11 - Free for peripherals / SCSI / NIC
isr_stub 44 # IRQ12 - PS2 Mouse
isr_stub 45 # IRQ13 - FPU / Coprocessor / Inter-processor
isr_stub 46 # IRQ14 - Primary ATA Hard Disk
isr_stub 47 # IRQ15 - Secondary ATA Hard Disk