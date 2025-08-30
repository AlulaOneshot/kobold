#ifndef X86_64_MM_H
#define X86_64_MM_H

#include <stdint.h>
#include <limine.h>

#define PAGE_SIZE 0x1000 // 4KB

void initPMM(struct limine_memmap_response *memmapResponse, uint64_t hhdm);
void *pmAlloc(uint64_t size);
void pmFree(void *ptr, uint64_t size);

#endif