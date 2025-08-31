#ifndef X86_64_MM_H
#define X86_64_MM_H

#include <stdint.h>
#include <limine.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <arch/x86_64/printf.h>
#include <math.h>

#define PAGE_SIZE 0x1000 // 4KB

extern uint64_t hhdmOffset;
extern uint8_t *bitmap;
extern uint64_t bitmapSize;

void initPMM(struct limine_memmap_response *memmapResponse, uint64_t hhdm);
void *pmAlloc(uint64_t size);
void pmFree(void *ptr, uint64_t size);

typedef uint64_t pagemap_t[512];

//TODO: PML5

#define PAGE_PRESENT   0b1
#define PAGE_WRITABLE  0b10
#define PAGE_USER 0b100
#define PAGE_PWT 0b1000
#define PAGE_PCD 0b10000
#define PAGE_ACCESSED  0b100000
#define PAGE_DIRTY 0b1000000 // Only for PML1
#define PAGE_PAT 0b10000000 // Only for PML1
#define PAGE_GLOBAL 0b100000000 // Only for PML1
// Avalible
// Reserved (0)
// bits 8-11 Avalible
// bits 12-51 Physical Address
// bits 52-62 Avalible
// bits 59-62 PK on PML1
// bit 63 No Execute

void initVMM(struct limine_memmap_response *memmapResponse, struct limine_kernel_address_response *kernelAddressResponse);
bool vmMapPage(pagemap_t *pagemap, uint64_t vaddr, uint64_t paddr, uint64_t flags);
void vmUnmapPage(pagemap_t *pagemap, uint64_t vaddr);
void vmLoad(pagemap_t *pml4);
void *vmGetPages(void *requestedAddress, size_t size);
void vmFreePages(void *address, size_t size);

#endif