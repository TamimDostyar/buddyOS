#ifndef SIMPLE_SUPERBLOCK_H
#define SIMPLE_SUPERBLOCK_H

typedef struct{
    uint32_t magic;          
    uint32_t block_size;     
    uint32_t total_blocks;   

    uint32_t dir_start_block;
    uint32_t dir_entry_count;

    uint32_t data_start_block;
} Superblock_t;

#endif
