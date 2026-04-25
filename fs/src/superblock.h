#ifndef SUPERBLOCKS
#define SUPERBLOCKS

typedef struct {

    // identity
    uint32_t magic;
    uint32_t version;

    // size based on geo
    uint64_t block_size;
    uint64_t total_block;
    uint64_t total_inode;

    // free space
    uint64_t free_block;
    uint64_t free_inode;

    // root of the system

    uint32_t root;
    // location
    uint64_t inode_table_start;   // Block where inode table begins
    uint64_t data_blocks_start;   // Block where data region begins
    uint64_t block_bitmap_start;  // Block where free-block bitmap lives
    uint64_t inode_bitmap_start;
    
    // status
    uint8_t  fs_state;      
    uint32_t mount_count;    
    uint64_t last_mount_time;
    uint64_t last_write_time;

} Blocks;


#endif