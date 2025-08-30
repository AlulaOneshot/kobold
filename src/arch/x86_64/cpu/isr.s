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
    .globl  isr_\num
    isr_\n:
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