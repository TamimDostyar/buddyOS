# Light Project 3: Physical and Virtual Memory Manager (TamimOS Week 3)

## Project Overview
This project introduces controlled memory ownership in kernel space.
You will implement physical allocation and a minimal virtual memory mapping strategy.

## Dependency on Projects 1 and 2

- Boot code must define known memory boundaries.
- Shell infrastructure can expose debug commands for allocator tests.
- UART output remains essential for diagnostics.

## Goal
Implement core memory services:

- kmalloc()
- kfree()

Then add a basic page-table-backed virtual memory model so processes can have isolated address spaces.

## What You Should Have After This

1. Distinguish physical memory tracking from virtual address translation.
2. Implement allocator metadata safely in kernel memory.
3. Detect and prevent common allocator faults (double free, invalid free, corruption).
4. Build a minimal mapping API for page-level virtual memory.
5. Validate memory behavior through deterministic kernel tests.

## Functional Requirements

1. Physical memory manager
- Track free/used memory with free-list or bitmap model.
- Support aligned allocation for page-sized requests.
- Support deallocation with metadata consistency checks.

2. Kernel allocator API
- Implement kmalloc(size_t size).
- Implement kfree(void *ptr).
- Define behavior for zero-size and null-pointer edge cases.

3. Virtual memory baseline
- Create and initialize a kernel page table structure.
- Provide API to map virtual page to physical frame.
- Provide API to unmap pages safely.
- Ensure at least one isolated address space structure per process abstraction.

4. Diagnostics
- Print allocator statistics (total, used, free).
- Add integrity check routine for allocator metadata.

## Suggested API Surface

- void mem_init(uintptr_t start, uintptr_t end)
- void *kmalloc(size_t size)
- void kfree(void *ptr)
- int map_page(vaddr_t va, paddr_t pa, uint32_t flags)
- int unmap_page(vaddr_t va)

Adapt names as needed, but keep responsibilities clear.

## What To Build

1. Memory subsystem source
- Physical frame tracking implementation.
- Kernel heap allocation implementation.
- Virtual mapping helpers and page-table structures.

2. Validation artifacts
- Test log showing allocate/free cycles.
- Evidence that invalid free paths are safely handled.
- Demonstration of at least two distinct virtual mappings.

3. Design notes
- 1 to 2 page markdown write-up explaining:
- Why you chose bitmap or free list.
- Time complexity trade-offs.
- Fragmentation risks and mitigation.

## Testing Requirements

1. Positive tests
- Allocate multiple blocks of varying sizes.
- Free in different order than allocation.
- Reallocate after free to prove reuse.

2. Negative tests
- kfree(NULL)
- Double free attempt
- Free pointer outside allocator range

3. Virtual memory tests
- Map two pages and verify no overlap in frame ownership.
- Unmap and remap without corruption.

## Performance and Safety Expectations

- No unbounded scan unless justified and documented.
- Metadata corruption should produce explicit failure path, not silent continuation.
- Keep interrupt-safety considerations documented, even if not fully implemented yet.

## Done Criteria

- kmalloc()/kfree() work for normal and edge cases.
- Invalid frees are handled safely.
- Virtual map/unmap works for at least two pages.

## Forward Link to Week 4

Your process scheduler will depend on per-process stack and address space setup.
The quality of this memory manager directly affects process stability.
