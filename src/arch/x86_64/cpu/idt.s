.text
.globl isr_common_handler

.macro pushaq
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rbx
    pushq %rbp
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
.endm

.macro popaq
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rbp
    popq %rbx
    popq %rdx
    popq %rcx
    popq %rax
.endm

.macro isr_stub num
    .globl isr_\num
isr_\num:
    pushq $0
    pushq $\num
    jmp isr_common_handler
.endm


.macro isr_stub_err num
    .globl isr_\num
isr_\num:
    pushq $\num
    jmp isr_common_handler
.endm

isr_stub 0

isr_common_handler:
    cli

    pushaq

    movq %cr0, %rax
    pushq %rax
    movq %cr2, %rax
    pushq %rax
    movq %cr3, %rax
    pushq %rax
    movq %cr4, %rax
    pushq %rax

    movq %rsp, %rdi
    call isrCommonC

    popq %rax
    popq %rax
    popq %rax
    popq %rax

    popaq

    addq $16, %rsp # Skip pushed error/vector codes

    sti
    iretq
