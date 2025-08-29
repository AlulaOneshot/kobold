.globl loadCS

loadCS: # NOTE: this does not work for ring switching
    pop %rax # Pop the return address
    push %rdi # Push the segment we are moving into
    push %rax # Push the return address
    retfq # Far return