#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void) {
    //TODO: Abort Message
    asm volatile("hlt");
	while (1) { }
	__builtin_unreachable();
}