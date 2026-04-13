# Project 2: Kernel Memory Allocator

## Prerequisites
- Project 1: Bootloader and Minimal C Kernel

## Goal
Implement dynamic kernel heap allocation for BuddyOS so kernel subsystems can request and release memory safely.

## What You Will Build
- `kmalloc` and `kfree` APIs for kernel-only use.
- Heap initialization over a fixed kernel heap region.
- Free-list allocator using first-fit placement.
- Block coalescing to reduce fragmentation.

## Required Features
1. Heap setup
- Reserve a heap region in linker script or kernel memory map.
- Initialize allocator metadata during kernel boot.

2. Allocation path
- First-fit scan of free list.
- Split larger blocks when useful.
- Return aligned pointers.

3. Free path
- Mark blocks free and reinsert correctly.
- Coalesce adjacent free blocks.

4. Safety checks
- Reject null or invalid frees gracefully.
- Add lightweight debug assertions in kernel builds.

## Suggested Structure
- `kernel/mm/heap.c`
- `kernel/mm/heap.h`
- `kernel/mm/kmalloc.c`
- `kernel/tests/mm_allocator_test.c`

## Testing Checklist
- Repeated allocate/free cycles for mixed sizes.
- Fragmentation scenario followed by successful coalescing.
- Large allocation fails safely when out of memory.
- Kernel remains stable after stress loop.

## Deliverable
A working `kmalloc`/`kfree` implementation used by BuddyOS kernel components.

## Next Step
Proceed to Project 3: Process Scheduler.
