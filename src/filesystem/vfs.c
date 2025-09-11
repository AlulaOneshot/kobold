#include <filesystem.h>

vfs_root_t root;
vfs_operations_t defaultOps = {
    .makeDir = vfsMakeDir,
    .makeFile = vfsMakeFile,
    .read = NULL,
    .write = NULL,
    .seek = NULL,
    .ioctl = NULL,
    .fileSize = NULL,
    .delete = NULL,
    .mountFilesystem = NULL
};

ssize_t vfsMakeDir(vfs_node_t *parent, const char *name, uint16_t permissions) {
    vfs_node_t *newDir = malloc(sizeof(vfs_node_t));
    if (!newDir) return VFS_MEMORY_ALLOCATION_FAILED; // Memory allocation failed

    newDir->children = malloc(sizeof(list_t));
    if (!newDir->children) {
        free(newDir);
        return VFS_MEMORY_ALLOCATION_FAILED; // Memory allocation failed
    }

    strcpy(newDir->name, name);
    newDir->permissions = permissions;
    newDir->type = VFS_NODE_TYPE_DIRECTORY;
    newDir->operations = &defaultOps;
    newDir->parent = parent;
    newDir->children->length = 0;
    newDir->children->head = NULL;
    newDir->children->tail = NULL;
    newDir->filesystem = VFS_FILESYSTEM_TYPE_VFS;
    newDir->internal = NULL;

    listPush(parent->children, newDir);

    return 0;
} 

ssize_t vfsMakeFile(vfs_node_t *parent, const char *name, uint16_t permissions) {
    vfs_node_t *file = malloc(sizeof(vfs_node_t));
    if (!file) return VFS_MEMORY_ALLOCATION_FAILED; // Memory allocation failed

    strcpy(file->name, name);
    file->permissions = permissions;
    file->type = VFS_NODE_TYPE_FILE;
    file->operations = &defaultOps;
    file->parent = parent;
    file->filesystem = VFS_FILESYSTEM_TYPE_VFS;
    file->internal = NULL;

    listPush(parent->children, file);

    return 0;
}

void initialiseVFS() {
    root.root = malloc(sizeof(vfs_node_t));
    root.root->children = malloc(sizeof(list_t));
    strcpy(root.root->name, "/");
    root.root->operations = &defaultOps;
    root.root->parent = NULL;
    root.root->permissions = 0;
    root.root->type = VFS_NODE_TYPE_DIRECTORY;
    root.root->children->length = 0;
    root.root->children->head = NULL;
    root.root->children->tail = NULL;
    root.root->internal = NULL;

    // Create /mnt directory for mounting other filesystems
    root.root->operations->makeDir(root.root, "mnt", 0);

    devFSMountFilesystem(root.root, "dev");
}

vfs_node_t *resolvePathVFS(const char *path) {
    if (strcmp(path, "/") == 0) {
        return root.root;
    }

    if (path[0] != '/') {
        return NULL; // Invalid path
    }

    path++; // Skip the leading '/'

    vfs_node_t *current = root.root;

    char *pathCopy = strdup(path);
    if (!pathCopy) {
        return NULL; // Memory allocation failed
    }

    char *token = strtok(pathCopy, "/");
    while (token && current) {
        if (current->type != VFS_NODE_TYPE_DIRECTORY) {
            free(pathCopy);
            return NULL; // Can't traverse into non-directory
        }

        list_node_t *childNode = current->children->head;
        vfs_node_t *next = NULL;

        while (childNode) {
            vfs_node_t *child = (vfs_node_t *)childNode->data;
            if (strcmp(child->name, token) == 0) {
                next = child;
                break;
            }
            childNode = childNode->next;
        }

        if (!next) {
            free(pathCopy);
            return NULL; // Path component not found
        }

        current = next;
        token = strtok(NULL, "/");
    }

    free(pathCopy);
    return current;
}