# Project 3: Process Scheduler

## Prerequisites
- Project 1: Bootloader and Minimal C Kernel
- Project 2: Kernel Memory Allocator

## Goal
Add process lifecycle and CPU scheduling to BuddyOS so multiple programs can run with controlled execution order.

## What You Will Build
- Process control block (PCB) model.
- Context switch mechanism between runnable processes.
- Round-robin scheduler.
- Core process syscalls: `fork`, `exec`, `wait`, `exit`.

## Required Features
1. Process model
- Define states such as ready, running, blocked, and terminated.
- Maintain per-process stack and register context.

2. Scheduler
- Timer-driven preemption or explicit yield-based switching.
- Fair round-robin selection from ready queue.

3. Lifecycle syscalls
- `fork`: create child process context.
- `exec`: replace process image.
- `wait`: block parent until child exits.
- `exit`: terminate and release process resources.

4. Robustness
- Prevent scheduler from selecting invalid or dead tasks.
- Handle process creation failure paths safely.

## Suggested Structure
- `kernel/proc/process.c`
- `kernel/proc/scheduler.c`
- `kernel/proc/context_switch.asm`
- `kernel/syscalls/process_syscalls.c`

## Testing Checklist
- Multiple CPU-bound test tasks rotate in round-robin order.
- Parent/child relationship works with `fork` and `wait`.
- `exec` successfully replaces a process image.
- Exited tasks are cleaned up without leaks.

## Deliverable
A BuddyOS kernel that can create, schedule, and terminate processes using a round-robin scheduler.

## Next Step
Proceed to Project 4: File System.
