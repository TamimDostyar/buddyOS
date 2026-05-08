#ifndef FILESYSTEMS
#define FILESYSTEMS

#include "superblock.h"

#define MAX_FILE_ENTRY 32
#define BLOCK_SIZE 512
#define MAX_DIR_ENTRIES 16
#define FS_MAGIC 0x12345678

typedef struct{
    char f_name[MAX_FILE_ENTRY];
    unsigned int size;
    unsigned int start_block;
    unsigned int used;
} FileEntry_t;

typedef struct {
    Superblock_t super;
    FileEntry_t* directory;
} fileSystemState_t;

typedef struct {
    FileEntry_t entries[MAX_DIR_ENTRIES];
} Directory_t;


// function prototypes!
void fs_mount(void);
int fs_create(const char* name, unsigned int size, const void* data);
int fs_read(const char* name, void* buffer);
int fs_delete(const char* name);
void fs_list(void);
// int name_equals(const char *a, const char *b);

#endif

