#pragma once

#include <sys.h>

typedef struct vfs_node vfs_node_t;
typedef struct vfs_open_file vfs_open_file_t;

/// @brief Reads from a file opened in the VFS.
/// @param file The file to read from
/// @param out The buffer to read into
/// @param size The amount of bytes to read at maximum
/// @param offset The offset to read at
/// @return The number of bytes read, or a negative error code
typedef ssize_t (*vfs_read_handler_t)(vfs_open_file_t *file, uint8_t *out, size_t size, size_t offset);

/// @brief Writes to a file opened in the VFS.
/// @param file The file to write to
/// @param out The buffer to write from
/// @param size The amount of bytes to write at maximum
/// @param offset The offset to write at
/// @return The number of bytes written, or a negative error code
typedef ssize_t (*vfs_write_handler_t)(vfs_open_file_t *file, const uint8_t *in, size_t size, size_t offset);

/// @brief Seeks to a position in a file opened in the VFS.
/// @param file The file to seek in
/// @param offset The offset to seek to
/// @return The new position in the file, or a negative error code
typedef ssize_t (*vfs_seek_handler_t)(vfs_open_file_t *file, size_t offset);

/// @brief Performs an ioctl operation on a file opened in the VFS.
/// @param file The file to perform the ioctl on
/// @param request The ioctl request code
/// @param arg The argument for the ioctl request
/// @return The result of the ioctl operation, or a negative error code
typedef ssize_t (*vfs_ioctl_handler_t)(vfs_open_file_t *file, int64_t request, uint64_t arg);

/// @brief Gets the size of a file opened in the VFS.
/// @param file The file to get the size of
/// @return The size of the file, or a negative error code
typedef ssize_t (*vfs_file_size_handler_t)(vfs_open_file_t *file);

/// @brief Deletes a file or directory node in the VFS.
/// @param node The node to delete
/// @return 0 on success, or a negative error code
typedef ssize_t (*vfs_delete_handler_t)(vfs_node_t *node);

/// @brief Creates a directory in the VFS.
/// @param parent The parent directory node
/// @param name The name of the new directory
/// @param permissions The permissions for the new directory
/// @return 0 on success, or a negative error code
typedef ssize_t (*vfs_make_dir_handler_t)(vfs_node_t *parent, const char *name, uint16_t permissions);

/// @brief Creates a file in the VFS.
/// @param parent The parent directory node
/// @param name The name of the new file
/// @param permissions The permissions for the new file
/// @return 0 on success, or a negative error code
typedef ssize_t (*vfs_make_file_handler_t)(vfs_node_t *parent, const char *name, uint16_t permissions);

/// @brief Mounts a filesystem as a child of a directory node in the VFS. Note that this is to be implemented by the filesystem being mounted itself, not the VFS.
/// @param parent The parent directory node to mount the filesystem on
/// @param name The name of the mount point
/// @return 0 on success, or a negative error code
typedef ssize_t (*vfs_mount_filesystem_handler_t)(vfs_node_t *parent, const char *name);

typedef struct {
    vfs_read_handler_t read;
    vfs_write_handler_t write;
    vfs_seek_handler_t seek;
    vfs_ioctl_handler_t ioctl;
    vfs_file_size_handler_t fileSize;
    vfs_delete_handler_t delete;
    vfs_make_dir_handler_t makeDir;
    vfs_make_file_handler_t makeFile;
    vfs_mount_filesystem_handler_t mountFilesystem;
} vfs_operations_t;

typedef enum {
    VFS_NODE_TYPE_FILE,
    VFS_NODE_TYPE_DIRECTORY,
    VFS_NODE_TYPE_SYMLINK,
    VFS_NODE_TYPE_CHAR_DEVICE,
    VFS_NODE_TYPE_BLOCK_DEVICE
} vfs_node_type_t;

typedef enum {
    VFS_FILESYSTEM_TYPE_DEVFS,
    VFS_FILESYSTEM_TYPE_VFS,
    VFS_FILESYSTEM_TYPE_UNKNOWN
} vfs_filesystem_t;

typedef struct vfs_node {
    char name[256];
    uint16_t permissions;
    vfs_node_type_t type;
    vfs_filesystem_t filesystem; // The filesystem this node belongs to
    vfs_operations_t *operations;
    struct vfs_node *parent; // Pointer to parent node, NULL if root. Note that NULL means this node IS the root, not a child of root.
    list_t *children;   // List of child nodes, only valid if type is VFS_NODE_TYPE_DIRECTORY
    void *internal;    // Filesystem-specific data
} vfs_node_t;

typedef struct vfs_open_file {
    vfs_node_t *node;  // The node this file represents
    size_t position;   // Current position in the file
    uint16_t flags;    // Flags used when opening the file (e.g., read, write, append)
    vfs_operations_t *operations; // Operations for this open file, usually same as node's operations
    void *internal;    // Filesystem-specific data for the open file
} vfs_open_file_t;

/// @brief The root node of the VFS
typedef struct {
    vfs_node_t *root;  // The root directory node of the VFS
} vfs_root_t;

#define VFS_OPERATION_NOT_SUPPORTED -1
#define VFS_OPERATION_FAILED -2
#define VFS_FILE_NOT_FOUND -3
#define VFS_MEMORY_ALLOCATION_FAILED -4
#define VFS_OPERATION_NOT_IMPLEMENTED -5

ssize_t vfsMakeDir(vfs_node_t *parent, const char *name, uint16_t permissions);
ssize_t vfsMakeFile(vfs_node_t *parent, const char *name, uint16_t permissions);

/// @brief Initialises the Virtual Filesystem (VFS).
void initialiseVFS();

static inline const char *getFSString(vfs_filesystem_t fs) {
    switch (fs) {
        case VFS_FILESYSTEM_TYPE_DEVFS:
            return "devfs";
        case VFS_FILESYSTEM_TYPE_VFS:
            return "vfs";
        default:
            return "unknown";
    }
}

/// @brief Resolves a path in the VFS.
/// @param path The path to resolve
/// @return A pointer to the resolved vfs_node_t, or NULL if not found
vfs_node_t *resolvePathVFS(const char *path);

ssize_t devFSRead(vfs_open_file_t *file, uint8_t *out, size_t size, size_t offset);
ssize_t devFSWrite(vfs_open_file_t *file, const uint8_t *in, size_t size, size_t offset);
ssize_t devFSSeek(vfs_open_file_t *file, size_t offset);
ssize_t devFSIoctl(vfs_open_file_t *file, int64_t request, uint64_t arg);
ssize_t devFSFileSize(vfs_open_file_t *file);
ssize_t devFSDelete(vfs_node_t *node);
ssize_t devFSMountFilesystem(vfs_node_t *parent, const char *name);