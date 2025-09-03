/*
    The Kobold Kernel
    pmm.c - The physical memory manager
*/

#include <arch/x86_64/mm.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limine.h>
#include <arch/x86_64/printf.h>
#include <math.h>
#include <string.h>
#include <bitmaps.h>

bitmap_t *bitmap;
/// @brief The highest addressable memory location
uint64_t highestAddress;
uint64_t bitmapSize;
/// @brief The number of free pages
uint64_t freePages;
uint64_t hhdmOffset;

void initPMM(struct limine_memmap_response *memmapResponse, uint64_t hhdm) {
    hhdmOffset = hhdm;

    struct limine_memmap_entry **memmap = memmapResponse->entries;
    uint64_t memmapEntries = memmapResponse->entry_count;

    // Do know that this printf is not a permament thing. Expect better debugging tools later.
    printf("Got %lu memory map entries\n", memmapEntries);

    // Get the highest address so we can get the bitmaps expected size
    for (uint64_t i = 0; i < memmapEntries; i++) {
        if (memmap[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        uint64_t topAddress = memmap[i]->base + memmap[i]->length;

        if (topAddress > highestAddress) {
            highestAddress = topAddress;
        }
    }

    printf("Highest address: 0x%lx\n", highestAddress);

    bitmapSize = ALIGN_UP((highestAddress / PAGE_SIZE) / 8, PAGE_SIZE); // In bytes

    printf("Bitmap size: %lu bytes (%lu pages)\n", bitmapSize, bitmapSize / PAGE_SIZE);

    // Now we need to find a place to put the bitmap
    for (uint64_t i = 0; i < memmapEntries; i++)
    {
        if (memmap[i]->type != LIMINE_MEMMAP_USABLE)
            continue;
        if (memmap[i]->length >= bitmapSize)
        {
            bitmap = (bitmap_t *)(memmap[i]->base + hhdmOffset);

            memset(bitmap, 0xFF, bitmapSize);

            memmap[i]->length -= bitmapSize;
            memmap[i]->base += bitmapSize;

            printf("Placed bitmap at 0x%lx\n", (uint64_t)bitmap - hhdmOffset);

            break;
        }
    }

    if (memmap == NULL) {
        printf("Failed to find space for bitmap\n");
        while (1);
    }

    // Find all usable pages and clear them
    for (uint64_t i = 0; i < memmapEntries; i++)
    {
        if (memmap[i]->type != LIMINE_MEMMAP_USABLE)
            continue;
        for (uint64_t j = 0; j < memmap[i]->length; j += PAGE_SIZE)
        {
			freePages++;
            bitmapClearBit(bitmap, (memmap[i]->base + j) / PAGE_SIZE);
        }
    }

    printf("Found %lu free pages\n", freePages);

    if ((freePages * 4096) / (1024 * 1024) < 64) {
        printf("Error: Less than 64MB of RAM detected\n");
        printf("Kobold has been shown to fail to boot with less than 64MB of ram\n");
        printf("Hanging");
        while (1) {
            asm volatile("hlt");
        }
    }
}

/// @brief Returns a pointer to a block of physical memory of at least size bytes. Returns NULL on failure.
/// @param size 
/// @return  
void *pmAlloc(uint64_t size) {
    if (size == 0) {
        return NULL;
    }

    uint64_t pages = ALIGN_UP(size, PAGE_SIZE) / PAGE_SIZE;

    if (pages > freePages) {
        return NULL;
    }

    uint64_t currentStreak = 0;

    for (uint64_t i = 0; i < (highestAddress / PAGE_SIZE); i++) {
        if (!bitmapGetBit(bitmap, i)) {
            currentStreak++;
        } else {
            currentStreak = 0;
        }

        if (currentStreak == pages) {
            // We found a big enough streak
            for (uint64_t j = 0; j < pages; j++) {
                bitmapSetBit(bitmap, i - j);
            }

            freePages -= pages;

            return (void *)((i - pages + 1) * PAGE_SIZE);
        }
    }

    return NULL;
}

void pmFree(void *ptr, uint64_t size) {
    if (ptr == NULL || size == 0) {
        return;
    }

    uint64_t pages = ALIGN_UP(size, PAGE_SIZE) / PAGE_SIZE;
    uint64_t index = (uint64_t)ptr / PAGE_SIZE;

    for (uint64_t i = 0; i < pages; i++) {
        bitmapClearBit(bitmap, index + i);
    }

    freePages += pages;
}