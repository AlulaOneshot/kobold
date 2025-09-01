#include <arch/x86_64/cpu.h>
// Thanks to cavOS for some reference stuff.

uint16_t configReadWordPCI(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) |
                        (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out the address
    outl(PCI_CONFIG_ADDRESS, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (uint16_t)((inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}