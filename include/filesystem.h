#pragma once

#include <sys.h>

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

typedef ssize_t (*vfs_seek_handler_t)(vfs_open_file_t *file, size_t offset);

typedef struct {
    vfs_read_handler_t read;
    vfs_write_handler_t write;
} vfs_operations_t;