#ifndef FILESYSTEMS
#define FILESYSTEMS

#define MAX_FILE_ENTRY 32
typedef struct FileEntry_t{
    char f_name[MAX_FILE_ENTRY];
    uint32_t size;
    uint32_t start_block;
} FileEntry_t;

typedef struct fileSystemState_t{
    superblock super;
    FileEntry_t* directory;

}fileSystemState_t;

// function prototypes
void fs_mount(void);
int fs_create(const char* name, uint32_t size, const void* data);
int fs_read(const char* name, void* buffer);
int fs_delete(const char* name);
void fs_list(void);

#endif

