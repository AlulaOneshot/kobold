#include <filesystem.h>

vfs_root_t root;
vfs_operations_t defaultOps;

void initialiseVFS() {
    root.root = malloc(sizeof(vfs_node_t));
    root.root->children = malloc(sizeof(list_t));
    root.root->name[0] = '/';
    root.root->name[1] = '\0';
    root.root->operations = &defaultOps;
    root.root->parent = NULL;
    root.root->permissions = 0;
    root.root->type = VFS_NODE_TYPE_DIRECTORY;
    root.root->children->length = 0;
    root.root->children->head = NULL;
    root.root->children->tail = NULL;
    root.root->internal = NULL;

    vfs_node_t *dev = malloc(sizeof(vfs_node_t));
    strncpy(dev->name, "dev", 256);
    dev->permissions = 0755;
    dev->type = VFS_NODE_TYPE_DIRECTORY;
    dev->operations = NULL; // No special operations for /dev
    dev->parent = NULL; // /dev is a child of root
    dev->children = malloc(sizeof(list_t));
    dev->children->length = 0;
    dev->children->head = NULL;
    dev->children->tail = NULL;
    dev->internal = NULL;
    listPush(root.root->children, dev);
}

static char *tokenize(char *s, char d) {
    static char* input = NULL;

    // Initialize the input string
    if (s != NULL)
        input = s;

    // Case for final token
    if (input == NULL)
        return NULL;

    // Stores the extracted string
    char* result = malloc(strlen(input) + 1);
    int i = 0;

    // Start extracting string and
    // store it in array
    for (; input[i] != '\0'; i++) {

        // If delimiter is not reached
        // then add the current character
        // to result[i]
        if (input[i] != d)
            result[i] = input[i];

        // Else store the string formed
        else {
            result[i] = '\0';
            input = input + i + 1;
            return result;
        }
    }

    // Case when loop ends
    result[i] = '\0';
    input = NULL;

    // Return the resultant pointer
    // to the string
    return result;
}

vfs_node_t *resolvePathVFS(const char *path) {
    if (strcmp(path, "/")) {
        return root.root;
    }

    if (path[0] != '/') {
        return NULL; // Invalid path
    }

    path++; // Skip the leading '/'

    vfs_node_t *current = root.root;
    while (current) {
        
    }
}