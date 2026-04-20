
#ifndef HEAP_P
#define HEAP_P

#include <stdint.h>
#include <stddef.h>

#define HEAP_SIZE (1024 * 1024)
#define HEAP_START 0x200000

typedef struct block_header{
    struct block_header *next;
    size_t size;
    int isFree;
} block_header_t;


void  heap_init(void);
void *kmalloc(size_t size);
void  kfree(void *ptr);

#endif