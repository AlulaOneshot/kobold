#ifndef X86_64_MM_H
#define X86_64_MM_H

/*
    The Kobold Kernel
    mm.h - Pretty much everything Memory Management related for x86_64.
    Includes:
        PMM
        VMM
*/

#include <stdint.h>
#include <limine.h>
#include <stdbool.h>
#include <string.h>
#include <arch/x86_64/printf.h>
#include <math.h>

#define PAGE_SIZE 0x1000 // 4KB

/// @brief The offset where the higher half kernel is mapped
extern uint64_t hhdmOffset;
/// @brief A bitmap representing used/free physical memory pages
extern uint8_t *bitmap;
/// @brief The size of the physical memory bitmap in bytes
extern uint64_t bitmapSize;

/// @brief Initialize the Physical Memory Manager
/// @param memmapResponse The Memory Map Returned by Limine
/// @param hhdm The Higher Half Direct Map Offset
void initPMM(struct limine_memmap_response *memmapResponse, uint64_t hhdm);
/// @brief Allocate physical memory
/// @param size Amount of physical memory to allocate in bytes
/// @return The physical address of the allocated memory, or NULL on failure
void *pmAlloc(uint64_t size);
/// @brief Free previously allocated physical memory
/// @param ptr The physical address of the memory to free
/// @param size The size of the memory block to free in bytes
void pmFree(void *ptr, uint64_t size);

/// @brief A page map (PML4, PML3, PML2, or PML1)
typedef uint64_t pagemap_t[512];

//TODO: PML5

#define PAGE_PRESENT 0b1
#define PAGE_WRITABLE 0b10
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

extern pagemap_t *kernelPML4;

/// @brief Initialize the Virtual Memory Manager
/// @param memmapResponse The Memory Map returned by Limine
/// @param kernelAddressResponse The Kernel Address Response returned by Limine
void initVMM(struct limine_memmap_response *memmapResponse, struct limine_kernel_address_response *kernelAddressResponse);
/// @brief Try to map a virtual address to a physical address in the given pagemap
/// @param pagemap The pagemap to modify
/// @param vaddr The virtual address to map
/// @param paddr The physical address to map to
/// @param flags The flags to apply
/// @return True on success, false on failure
bool vmMapPage(pagemap_t *pagemap, uint64_t vaddr, uint64_t paddr, uint64_t flags);
/// @brief Unmap a virtual address in the given pagemap
/// @param pagemap The pagemap to modify
/// @param vaddr The virtual address to unmap
void vmUnmapPage(pagemap_t *pagemap, uint64_t vaddr);
/// @brief Load the given pagemap into the CR3 register
/// @param pml4 The pagemap to load
void vmLoad(pagemap_t *pml4);
/// @brief Map a range of physical memory into the given pagemap
/// @param pagemap The pagemap to modify
/// @param paddr The starting physical address to map
/// @param size THe size of the range to map in bytes
/// @return The virtual address of the mapped range, or NULL on failure
void *vmMapPageRange(pagemap_t *pagemap, uint64_t paddr, size_t size);
/// @brief Unmap a range of virtual memory in the given pagemap
/// @param pagemap The pagemap to modify
/// @param address The starting virtual address to unmap
/// @param size The size of the range to unmap in bytes
void vmUnmapPageRange(pagemap_t *pagemap, void *address, size_t size);
/// @brief Get Some pages of virtual memory
/// @param requestedAddress The requested virtual address, or NULL for any address
/// @param size The size of the range of memory to allocate in bytes
/// @return The virtual address of the allocated memory, or NULL on failure
void *vmGetPages(void *requestedAddress, size_t size);
/// @brief Free Some pages of virtual memory
/// @param address The virtual address of the memory to free
/// @param size The size of the range of memory to free in bytes
void vmFreePages(void *address, size_t size);
/// @brief Checks if a page is present in a page table
/// @param vaddr The virtual address to check
/// @return True if the page is mapped, false otherwise
bool isPageMapped(uint64_t vaddr);
/// @brief Get the physical address mapped to a virtual address in a given pagemap
/// @param pagemap The pagemap to search
/// @param vaddr The virtual address to look up
/// @return The physical address, or 0 if not mapped
uint64_t vmGetPhysical(pagemap_t *pagemap, uint64_t vaddr);

#endif