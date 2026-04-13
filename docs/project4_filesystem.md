# Project 4: File System

## Prerequisites
- Project 1: Bootloader and Minimal C Kernel
- Project 2: Kernel Memory Allocator
- Project 3: Process Scheduler

## Goal
Implement BuddyOS persistent storage with a simple on-disk file system supporting files and directories.

## What You Will Build
- Disk layout with superblock, inode table, and data blocks.
- Free-space bitmap management.
- Directory entries and path lookup.
- Basic file operations: `open`, `read`, `write`, `close`, `mkdir`, `unlink`.

## Required Features
1. Disk and metadata layout
- Define a stable on-disk format.
- Initialize format routine for clean disk images.

2. Inode and directory logic
- Map file metadata to inodes.
- Resolve absolute and relative paths through directories.

3. File I/O
- Read and write data blocks.
- Track file size updates and handle append/overwrite.

4. Error handling
- Return consistent error codes for invalid paths and permissions.
- Avoid metadata corruption on failed operations.

## Suggested Structure
- `kernel/fs/superblock.c`
- `kernel/fs/inode.c`
- `kernel/fs/bitmap.c`
- `kernel/fs/dir.c`
- `kernel/fs/file.c`
- `kernel/fs/block_device.c`

## Testing Checklist
- Format then mount disk successfully.
- Create nested directories and list contents.
- Create/write/read/delete multiple files.
- Reboot and confirm data persists on disk image.

## Deliverable
A working BuddyOS file system integrated with kernel storage interfaces.

## Next Step
Proceed to Project 5: System Calls.
