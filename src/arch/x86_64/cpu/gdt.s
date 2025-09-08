.globl loadCS

loadCS: # NOTE: this does not work for ring switching
    pop %rax # Pop the return address
    mov %di, %dx
    push %dx // Push 16 Bit Segment Selector
    push %rax # Push the return address
    retfq # Far return