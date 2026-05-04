#include "debug.h"
#include "fs.h"
#include "superblock.h"

extern uint8_t disk_data[];
fileSystemState_t fs;
#define FS_MAGIC 0x12345678

void disk_read_block(int block_number, void *buffer){
    int offset = block_number * BLOCK_SIZE;
    memcpy(buffer, &disk_data[offset], BLOCK_SIZE);
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

void fs_mount_fail(void){
    kprintf_str("Mounting failed");
}


void fs_mount(void){
    disk_read_block(0, &fs.super);

    if (fs.super.magic != FS_MAGIC){
        fs_mount_fail();
        return;
    }

    Directory_t root_dir;
    disk_read_block(fs.super.dir_start_block, &root_dir);
    fs.directory = root_dir.entries;

}

// void fs_unmount(void){

// }
