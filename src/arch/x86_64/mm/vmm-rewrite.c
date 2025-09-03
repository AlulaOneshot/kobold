#include <arch/x86_64/mm.h>
#include <math.h>

extern char limineStart[], limineEnd[];
extern char textStart[], textEnd[];
extern char dataStart[], dataEnd[];

pagemap_t *kernelPML4;

vmmaps_t *maps;

void vmMapPage(pagemap_t *map, void *virtual, uint64_t phys, uint64_t )

void vmmInit() {
    uint64_t limineStartAligned = ALIGN_DOWN((uint64_t)&limineStart, PAGE_SIZE);
    uint64_t limineEndAligned = ALIGN_UP((uint64_t)&limineEnd, PAGE_SIZE);
    
    uint64_t textStartAligned = ALIGN_DOWN((uint64_t)&textStart, PAGE_SIZE);
    uint64_t textEndAligned = ALIGN_UP((uint64_t)&textEnd, PAGE_SIZE);
    
    uint64_t dataStartAligned = ALIGN_DOWN((uint64_t)&dataStart, PAGE_SIZE);
    uint64_t dataEndAligned = ALIGN_UP((uint64_t)&dataEnd, PAGE_SIZE);

    uint64_t bitmapStartAligned = ALIGN_DOWN((uint64_t)bitmap, PAGE_SIZE);
    uint64_t bitmapEndAligned = ALIGN_UP((uint64_t)bitmap + bitmapSize, PAGE_SIZE);

    
}