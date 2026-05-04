#include "debug.h"
#include <stddef.h>
#include "fs.h"
#include "superblock.h"

uint8_t disk_data[BLOCK_SIZE * 1024]; // Basic disk array
fileSystemState_t fs;
static Directory_t fs_dir;

static size_t str_len(const char *s){
    size_t n = 0;
    if (!s){
        return 0;
    }
    while (s[n] != '\0'){
        n++;
    }
    return n;
}

static int name_equals(const char *a, const char *b){
    size_t i = 0;
    if (!a || !b){
        return 0;
    }
    while (i < MAX_FILE_ENTRY){
        if (a[i] != b[i]){
            return 0;
        }
        if (a[i] == '\0'){
            return 1;
        }
        i++;
    }
    return 1;
}

static void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

void disk_read_block(int block_number, void *buffer){
    int offset = block_number * BLOCK_SIZE;
    memcpy(buffer, &disk_data[offset], BLOCK_SIZE);
}

void disk_write_block(int block_number, const void *buffer){
    int offset = block_number * BLOCK_SIZE;
    memcpy(&disk_data[offset], buffer, BLOCK_SIZE);
}

void fs_format(void){
    fs.super.magic = FS_MAGIC;
    fs.super.block_size = BLOCK_SIZE;
    fs.super.total_blocks = 1024;
    fs.super.dir_start_block = 1;
    fs.super.dir_entry_count = MAX_DIR_ENTRIES;
    fs.super.data_start_block = 2;

    disk_write_block(0, &fs.super);
    for (int i = 0; i < MAX_DIR_ENTRIES; i++){
        fs_dir.entries[i].used = 0;
    }
    
    disk_write_block(fs.super.dir_start_block, &fs_dir);
}

void fs_mount(void){
    disk_read_block(0, &fs.super);

    if (fs.super.magic != FS_MAGIC){
        kprintf_str("Unformatted disk detected. Auto-formatting...\n");
        fs_format();
        disk_read_block(0, &fs.super);
        
        if (fs.super.magic != FS_MAGIC){
            kprintf_str("Format failed!\n");
            return;
        } else {
            kprintf_str("Format successful!\n");
        }
    } else{
        kprintf_str("Mounting failed");
    }

    if (fs.super.block_size != BLOCK_SIZE ||
        fs.super.dir_entry_count > MAX_DIR_ENTRIES ||
        fs.super.dir_start_block >= fs.super.total_blocks){
        kprintf_str("mounting failed because size is not fine");
        return;
    }

    disk_read_block(fs.super.dir_start_block, &fs_dir);
    fs.directory = fs_dir.entries;
}

void fs_unmount(void){
    fs.directory = NULL;
}

void fs_list(void){
    if (!fs.directory){
        return;
    }
    for (int i = 0; i < MAX_DIR_ENTRIES; i ++){
        if (fs.directory[i].used){
            kprintf_str(fs.directory[i].f_name);
        }
    }
}
int fs_create(const char* name, unsigned int size, const void* data){
    if (!fs.directory || !name){
        return -1;
    }

    if (str_len(name) >= MAX_FILE_ENTRY){
        return -1;
    }

    for (int i = 0; i < MAX_DIR_ENTRIES; i++){
        if (fs.directory[i].used && name_equals(fs.directory[i].f_name, name)){
            return -1;
        }
    }

    for (int i = 0; i < MAX_DIR_ENTRIES; i++){
        if (!fs.directory[i].used){
            size_t j = 0;
            while (j < MAX_FILE_ENTRY){
                fs.directory[i].f_name[j] = name[j];
                if (name[j] == '\0'){
                    break;
                }
                j++;
            }
            if (j == MAX_FILE_ENTRY){
                fs.directory[i].f_name[MAX_FILE_ENTRY - 1] = '\0';
            }
            fs.directory[i].size = size;
            fs.directory[i].start_block = 0;
            fs.directory[i].used = 1;
            (void)data;
            return 0;
        }
    }
    return -1;
}

int fs_delete(const char* name){
    if (!fs.directory || !name){
        return -1;
    }

    for (int i = 0; i < MAX_DIR_ENTRIES; i++){
        if (fs.directory[i].used && name_equals(fs.directory[i].f_name, name)){
            fs.directory[i].used = 0;
            return 0;
        }
    }
    return -1;
}