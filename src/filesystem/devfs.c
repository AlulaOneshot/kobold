#include <filesystem.h>

vfs_operations_t devFSOps = {
    .makeDir = NULL,
    .makeFile = NULL,
    .read = devFSRead,
    .write = devFSWrite,
    .seek = devFSSeek,
    .ioctl = devFSIoctl,
    .fileSize = devFSFileSize,
    .delete = devFSDelete,
    .mountFilesystem = devFSMountFilesystem
};

ssize_t devFSRead(vfs_open_file_t *file, uint8_t *out, size_t size, size_t offset) {
    return VFS_OPERATION_NOT_IMPLEMENTED; // Not implemented yet
}

ssize_t devFSWrite(vfs_open_file_t *file, const uint8_t *in, size_t size, size_t offset) {
    return VFS_OPERATION_NOT_IMPLEMENTED; // Not implemented yet
}

ssize_t devFSSeek(vfs_open_file_t *file, size_t offset) {
    if (!file || !file->node) {
        return VFS_OPERATION_FAILED; // Invalid file
    }
    if (offset > file->node->internal) {
        return VFS_OPERATION_FAILED; // Offset out of bounds
    }
    file->position = offset;
    return 0; // Success
}

ssize_t devFSIoctl(vfs_open_file_t *file, int64_t request, uint64_t arg) {
    return VFS_OPERATION_NOT_IMPLEMENTED; // Not implemented yet
}

ssize_t devFSFileSize(vfs_open_file_t *file) {
    return VFS_OPERATION_NOT_IMPLEMENTED; // Not implemented yet.
}

ssize_t devFSDelete(vfs_node_t *node) {
    return VFS_OPERATION_NOT_IMPLEMENTED; // Not implemented yet. If called, would gracefully remove said device, unless essential to system operation. For example, deleting a keyboard device would kind of just remove the keyboard from the userspace perspective, but the keyboard would still work for system input.
}

ssize_t devFSMountFilesystem(vfs_node_t *parent, const char *name) {
    if (!parent || parent->type != VFS_NODE_TYPE_DIRECTORY) {
        return VFS_OPERATION_FAILED; // Invalid parent
    }
    vfs_node_t *devDir = malloc(sizeof(vfs_node_t));
    if (!devDir) {
        return VFS_MEMORY_ALLOCATION_FAILED; // Memory allocation failed
    }
    devDir->children = malloc(sizeof(list_t));
    if (!devDir->children) {
        free(devDir);
        return VFS_MEMORY_ALLOCATION_FAILED; // Memory allocation failed
    }

    strcpy(devDir->name, name);
    devDir->permissions = 0; // Default permissions, can be changed later
    devDir->type = VFS_NODE_TYPE_DIRECTORY;
    devDir->operations = &devFSOps;
    devDir->parent = parent;
    devDir->children->length = 0;
    devDir->children->head = NULL;
    devDir->children->tail = NULL;
    devDir->filesystem = VFS_FILESYSTEM_TYPE_DEVFS;
    devDir->internal = NULL; // No special internal data for the base devfs directory, devices will have their own internal data

    listPush(parent->children, devDir);
}