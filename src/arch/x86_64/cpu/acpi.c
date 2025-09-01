#include <uacpi/kernel_api.h>
#include <uacpi/acpi.h>
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/mm.h>
// Start with some UACPI Glue

uint64_t rsdpAddress = 0;

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address) {
    if (rsdpAddress == 0) {
        return UACPI_STATUS_NOT_FOUND;
    }
    *out_rsdp_address = rsdpAddress;
    return UACPI_STATUS_OK;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
    return vmMapPageRange(kernelPML4, addr, len);
}

void uacpi_kernel_unmap(void *addr, uacpi_size len) {
    vmUnmapPageRange(kernelPML4, addr, len);
}

void uacpi_kernel_log(uacpi_log_level level, const uacpi_char *msg) {
    // For simplicity, we ignore the log level for now
    printf("[uACPI] %s\n", msg);
}

uacpi_status uacpi_kernel_pci_device_open(
    uacpi_pci_address address, uacpi_handle *out_handle
) {
    uacpi_pci_address *addr = (uacpi_pci_address *)vmGetPages(NULL, sizeof(uacpi_pci_address));
    if (!addr) return UACPI_STATUS_OUT_OF_MEMORY;
    *addr = address;
    *out_handle = (uacpi_handle *)addr;
}

void uacpi_kernel_pci_device_close(uacpi_handle handle) {
    vmUnmapPageRange(kernelPML4, handle, sizeof(uacpi_pci_address));
}

uacpi_status uacpi_kernel_pci_read8(uacpi_handle device, uacpi_size offset, uacpi_u8 *value) {
    uacpi_pci_address *address = (uacpi_pci_address *)device;
    
    *value = ConfigReadWord(address->bus, address->device, address->function,
                          offset & ~1);
    *value = (*value >> ((offset & 1) * 8)) & 0xFF;
}

// End UACPI Glue

void initACPI(uint64_t address) {
    rsdpAddress = address;
}

