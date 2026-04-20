# Kernel Memory Allocator

A simple first-fit heap allocator for the BuddyOS kernel. Provides `kmalloc` and `kfree` for dynamic kernel memory allocation over a fixed heap region.

## Files

```
alloc/
  src/
    heap.h      - Block header struct, constants, and function declarations
    heap.c      - Heap initialization and buffer definition
    kmalloc.c   - kmalloc, kfree, and merge implementations
test/
  mm_allecatorTest.c - Test suite
```

## Memory Layout

The heap starts at a fixed address (`HEAP_START = 0x200000`) and is 1MB in size. Each allocation is preceded by a `block_header_t` in memory:

```
HEAP_START
    |
    v
[ block_header_t | user data ] -> [ block_header_t | user data ] -> ... -> NULL
```

Each `block_header_t` contains:
- `next` — pointer to the next block in the list
- `size` — size of the user data region (not including the header)
- `isFree` — `1` if the block is available, `0` if allocated

---

## API

### `void heap_init(void)`

Initializes the heap before any allocation. Must be called once during kernel boot before `kmalloc` or `kfree` are used.

Sets up the entire heap as a single large free block:
- `size` = `HEAP_SIZE - sizeof(block_header_t)`
- `isFree` = `1`
- `next` = `NULL`

---

### `void *kmalloc(size_t size)`

Allocates `size` bytes of kernel memory. Returns a pointer to the usable data region, or `NULL` on failure.

**Behavior:**
- Returns `NULL` if `size` is `0`
- Walks the free list from `HEAP_START` using first-fit strategy
- If a free block is found that is large enough and has room to split, it is split into two blocks: one of exactly `size` bytes (allocated) and one with the remainder (free)
- Marks the allocated block as not free and returns a pointer to the byte immediately after its header

**Returns:** pointer to allocated memory, or `0` if no suitable block exists (out of memory)

---

### `void kfree(void *ptr)`

Frees a previously allocated block. The pointer must have been returned by `kmalloc`.

**Behavior:**
- Returns immediately if `ptr` is `NULL`
- Steps back one `block_header_t` from `ptr` to recover the block header
- Marks the block as free (`isFree = 1`)
- Calls `merge()` to coalesce any adjacent free blocks

---

### `void merge(void)`

Scans the entire free list from `HEAP_START` and merges consecutive free blocks into a single larger block. Called automatically by `kfree`.

**Why this is needed:** After multiple alloc/free cycles, the heap can become fragmented into many small free blocks. Even if total free memory is large enough for a new allocation, no single block may be big enough. Merging fixes this.

**Behavior:**
- Walks from `HEAP_START` to end of list
- When two adjacent blocks are both free, absorbs the next block into the current one:
  - `current->size += sizeof(block_header_t) + next->size`
  - `current->next = next->next`
- Repeats until no two adjacent free blocks remain

---

## Testing

The test suite lives in `test/mm_allecatorTest.c`. Because the allocator uses a fixed kernel address (`0x200000`) that is not mapped in a normal process, tests use a compile-time flag to swap in a regular stack buffer instead.

### Running the tests

From the `test/` directory:

```bash
clang -DTEST -o mem mm_allecatorTest.c ../alloc/src/heap.c ../alloc/src/kmalloc.c
./mem
```

The `-DTEST` flag activates a `heap_buffer` array in place of the fixed kernel address, making the allocator safe to run on macOS.

### What is tested

| Test | Description |
|------|-------------|
| `test_basic_alloc` | Single allocation returns a non-null pointer |
| `test_zero_alloc` | Allocating 0 bytes returns null |
| `test_multiple_allocs` | Three allocations return distinct non-null pointers |
| `test_reuse_after_free` | A freed block can be reallocated |
| `test_coalescing` | After freeing multiple adjacent blocks, a larger allocation succeeds |
| `test_repeated_cycles` | 100 alloc/free cycles all succeed |
| `test_out_of_memory` | Requesting more than heap size returns null |
| `test_null_free` | Freeing a null pointer does not crash |


```
  PASS: basic alloc returns non-null
  PASS: zero alloc returns null
  ...
110/110 tests passed
```
