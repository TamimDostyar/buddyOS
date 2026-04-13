# Light Project 4: BuddyOS On-Disk File System

## Project Goal
Build a file system implementation for **BuddyOS**: layout on a virtual disk (or RAM disk), with operations exercised through BuddyOS’s file or block interface—not through mounting inside another OS.

## What I Will Build

- create
- read
- write
- delete
- mkdir
- list directory

## Required Features

1. Integration surface
- Connect the FS to BuddyOS via a **block device**, **VFS hooks**, or a dedicated test server in the kernel, as your architecture defines.
- No dependency on external “userspace FS bridge” frameworks.

2. Storage model
- Back data with one virtual disk image (or contiguous region).
- Manage free space with a block bitmap.

3. Metadata model
- Implement inode-like structures.
- Implement directory entries.
- Track file sizes and block locations.

4. File and directory operations
- create/read/write/delete files.
- create directories and list contents.

5. Error handling
- Return consistent BuddyOS error codes for invalid paths and operations.

## Suggested Structure

- fs/fs_main.c (or vfs glue)
- fs/inode.c
- fs/bitmap.c
- fs/dir.c
- fs/block_io.c

## Testing Checklist

- Format or open disk succeeds.
- mkdir and list behave correctly.
- create/write/read/delete works for multiple files.
- Data remains correct across multiple operations.
- Free block bitmap updates correctly.

## OSTEP Connection

This project maps to inodes, directories, allocation bitmaps, and file-system layout.

## Deliverable

A BuddyOS file-system implementation usable from your kernel or user-mode FS layer, backed by the virtual disk model.
