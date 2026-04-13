# Project 6: Shell

## Prerequisites
- Project 1: Bootloader and Minimal C Kernel
- Project 2: Kernel Memory Allocator
- Project 3: Process Scheduler
- Project 4: File System
- Project 5: System Calls

## Goal
Build a command-line shell for BuddyOS that uses your own process and file syscalls.

## What You Will Build
- Command parser for executable + arguments.
- External command launcher via BuddyOS syscalls.
- Built-ins: `cd`, `pwd`, `history`, `exit`.
- I/O redirection (`<`, `>`).
- Single pipeline support (`|`).

## Required Features
1. Command execution
- Parse input into program name and arguments.
- Launch user programs and wait for foreground completion.

2. Built-ins
- `cd`, `pwd`, `history`, and `exit` run inside shell process.

3. I/O features
- Redirect input and output using file descriptor syscalls.
- Support one pipeline connecting two commands.

4. Stability
- Ignore empty input cleanly.
- Report unknown commands without crashing.
- Continue prompt loop until `exit`.

## Suggested Structure
- `userspace/shell/main.c`
- `userspace/shell/parser.c`
- `userspace/shell/exec.c`
- `userspace/shell/builtins.c`
- `userspace/shell/history.c`

## Testing Checklist
- Built-ins behave correctly.
- External programs run and return exit status.
- Redirection reads/writes expected files.
- Pipeline connects producer and consumer commands.
- Invalid syntax is handled gracefully.

## Deliverable
A working BuddyOS shell that runs on top of your completed kernel, syscall, process, and file-system stack.
