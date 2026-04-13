# Project 5: System Calls

## Prerequisites
- Project 1: Bootloader and Minimal C Kernel
- Project 2: Kernel Memory Allocator
- Project 3: Process Scheduler
- Project 4: File System

## Goal
Create the user-mode to kernel-mode interface for BuddyOS so user programs can safely access kernel services.

## What You Will Build
- Syscall entry mechanism (interrupt or software trap).
- Syscall dispatch table and handler implementations.
- ABI contract for arguments, return values, and errors.
- User-mode syscall wrappers and tests.

## Required Features
1. Entry and dispatch
- Route user calls through a single kernel entry point.
- Validate syscall number and arguments before dispatch.

2. Core syscall set
- Process calls: `fork`, `exec`, `wait`, `exit`.
- File calls: `open`, `read`, `write`, `close`.
- Optional utility call like `buddyos_info` for kernel status.

3. User-mode integration
- Provide libc-style wrappers or direct stubs.
- Build at least one user program that exercises multiple syscalls.

4. Safety
- Ensure invalid pointers or numbers fail without kernel crash.
- Return documented error codes.

## Suggested Structure
- `kernel/syscalls/entry.asm`
- `kernel/syscalls/dispatch.c`
- `kernel/syscalls/process.c`
- `kernel/syscalls/fs.c`
- `userspace/lib/syscall.c`
- `userspace/tests/syscall_smoke.c`

## Testing Checklist
- User program enters kernel and returns correctly.
- Valid syscall paths produce expected behavior.
- Invalid syscall usage returns errors safely.
- Kernel remains stable under repeated syscall stress.

## Deliverable
A stable BuddyOS syscall interface used by user-mode programs.

## Next Step
Proceed to Project 6: Shell.
