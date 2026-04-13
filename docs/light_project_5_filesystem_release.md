# Light Project 5: BuddyOS Kernel Syscall and Kernel Info

## Project Goal
Extend **BuddyOS** at the user–kernel boundary: add a custom system call and a small, documented way to read kernel information (no loadable module framework from another OS).

## What I Will Build

- syscall implementation in the BuddyOS kernel
- registration in your syscall table / trap handler for the target architecture
- user-mode test program that invokes the new syscall
- optional: simple kernel info export (for example a `buddyos_info` syscall that fills a buffer, or formatted output on a debug channel)

## Required Features

1. Kernel entry
- Implement the syscall in the BuddyOS source tree.
- Assign a syscall number consistent with your ABI documentation.
- Rebuild the BuddyOS kernel and boot it (emulator or hardware) as you do for other milestones.

2. Observable behavior
- The syscall returns predictable data or status (for example version string, tick count, or a test magic value).
- Errors from invalid arguments are handled cleanly.

3. User-space caller
- Write a C program linked for BuddyOS that invokes the new syscall.
- Validate return values and error handling.

4. Documentation
- Keep build and run instructions clear and reproducible.
- Document the syscall number, arguments, and any struct layout.

## Suggested Structure

- kernel/syscall_buddyos.c (or merged into your syscall dispatch)
- kernel/userspace/call_buddyos_syscall.c
- notes or patches under `kernel/patches/` if you track ABI changes

## Testing Checklist

- Kernel builds and boots.
- Syscall returns expected value in the happy path.
- User test program runs on BuddyOS and exercises the syscall.
- Invalid inputs fail without crashing the kernel.

## OSTEP Connection

This project maps to the user–kernel boundary, system call flow, and kernel-level programming.

## Deliverable

A BuddyOS kernel build where the custom syscall exists and is callable from BuddyOS user mode, with a short runbook for verification.
