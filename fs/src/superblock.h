#include <stdlib.h>

#ifndef SUPERBLOCKS
#define SUPERBLOCKS


typedef struct __super_block_t{
    // identity  and total number for partition
    uint32_t magic;

    uint32_t sector_count;
    uint32_t inode_count;

    // total size from partition
    uint32_t partition_lba_base;
    uint32_t block_bitmap_lba;
    uint32_t block_bitmap_sectors;

    // bitmap location

    // lba is the logical block address
    // and inode is the datastructure where it stores things like read/write modification date....
    uint32_t inode_bitmap_lba;
    uint32_t inode_bitmap_sectors;
    uint32_t inode_table_lba;
    uint32_t inode_table_sects;

    uint32_t data_start_lba;
    uint32_t root_inode_no;
    uint32_t dir_entry_size;


} __attribute__((packed)) super_block_t;

// the logic is inspired from JackOS
#endif