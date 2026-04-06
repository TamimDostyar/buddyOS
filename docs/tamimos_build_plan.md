# TamimOS Build Plan

This roadmap defines a 5-week build where each project is a dependent subsystem.
The complete sequence composes a lightweight ARM operating system.

## Dependency Graph

1. Week 1 Bootloader and UART
2. Week 2 Shell and Basic I/O
3. Week 3 Memory Manager
4. Week 4 Process Scheduler
5. Week 5 File System and Release

Each milestone must preserve prior functionality while introducing new kernel responsibilities.

## Weekly Progression Summary

1. Week 1: establish first code execution and serial heartbeat.
2. Week 2: build interactive control loop through shell commands.
3. Week 3: establish memory ownership and address mapping.
4. Week 4: introduce process abstraction and preemptive scheduling.
5. Week 5: add storage, syscall boundary, and ship TamimOS v1.0.

## Assignment Files

- Project 1 spec: light_project_1_bootloader_uart.md
- Project 2 spec: light_project_2_shell_io.md
- Project 3 spec: light_project_3_memory_manager.md
- Project 4 spec: light_project_4_process_scheduler.md
- Project 5 spec: light_project_5_filesystem_release.md

## Capstone Exit Criteria

TamimOS v1.0 is complete when the system can:

1. Boot on ARM emulation and print startup banner.
2. Accept shell commands via UART.
3. Allocate/free kernel memory with stable metadata.
4. Schedule at least two processes with timer preemption.
5. Persist and retrieve files on a RAM disk via syscalls.
