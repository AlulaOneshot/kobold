.globl setupNewStack
.extern initStage2

setupNewStack:
    mov %rdi, %rsp
    jmp initStage2
    