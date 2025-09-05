#include <arch/x86_64/cpu.h>
#include <arch/x86_64/mm.h>
#include <uacpi/kernel_api.h>

// We start With UACPI Glue

uint64_t rsdpAddress = 0;

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address) {
    if (rsdpAddress == 0) {
        return UACPI_STATUS_NOT_FOUND;
    }
    *out_rsdp_address = rsdpAddress;
    return UACPI_STATUS_OK;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
    uint64_t originalAddr = addr;
    addr = ALIGN_DOWN(originalAddr, PAGE_SIZE);
    len += originalAddr - addr;
    len = ALIGN_UP(len, PAGE_SIZE);
    uint64_t vaddr = (uint64_t)vmGetSpace(addr, len);
    if (vaddr == 0) {
        return NULL;
    }
    return (void *)(vaddr + (originalAddr - addr)); // Add offset
}

void uacpi_kernel_unmap(void *addr, uacpi_size len) {
    uint64_t originalAddr = (uint64_t)addr;
    addr = (void *)ALIGN_DOWN(originalAddr, PAGE_SIZE);
    len += originalAddr - (uint64_t)addr;
    len = ALIGN_UP(len, PAGE_SIZE);
    vmUnmapRange(kernelPML4, addr, len);
}

void uacpi_kernel_log(uacpi_log_level level, const uacpi_char *message) {
    switch (level)
    {
        case UACPI_LOG_DEBUG:
            printf("[UACPI DEBUG] %s", message);
            break;
        case UACPI_LOG_ERROR:
            printf("[UACPI ERROR] %s", message);
            break;
        case UACPI_LOG_INFO:
            printf("[UACPI INFO] %s", message);
            break;
        case UACPI_LOG_TRACE:
            printf("[UACPI TRACE] %s", message);
            break;
        case UACPI_LOG_WARN:
            printf("[UACPI WARN] %s", message);
            break;
    }
}

uacpi_status uacpi_kernel_pci_device_open(uacpi_pci_address address, uacpi_handle *out_handle) {
    pci_device_t *device = malloc(sizeof(pci_device_t));
    device->segment = address.segment;
    device->bus = address.bus;
    device->slot = address.device;
    device->function = address.function;

    // Comment copied bc its literally the same lol
    // TODO: this should not just create a device and push it,
    // it REALLY needs to first check if we are aware of such a device
    addPCIDevice(device);

    *out_handle = device;

    return UACPI_STATUS_OK;
}

void uacpi_kernel_pci_device_close(uacpi_handle handle) {
    //TODO: Remove the device
    // For now, we just ignore it.
}

uacpi_status uacpi_kernel_pci_read8(uacpi_handle handle, uacpi_size offset, uacpi_u8 *value) {
    pci_device_t *device = (pci_device_t *)handle;
    *value = readPCIConfig8(device, offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_read16(uacpi_handle handle, uacpi_size offset, uacpi_u16 *value) {
    pci_device_t *device = (pci_device_t *)handle;
    *value = readPCIConfig16(device, offset);
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_read32(uacpi_handle handle, uacpi_size offset, uacpi_u32 *value) {
    pci_device_t *device = (pci_device_t *)handle;
    *value = readPCIConfig32(device, offset);
    return UACPI_STATUS_OK;
}

