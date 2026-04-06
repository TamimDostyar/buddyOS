# Light Project 5: File System, Syscalls, and TamimOS v1.0 Release (Week 5)

## Project Overview
This capstone integrates all prior subsystems into one working release.
You will implement a flat file system on a RAM disk, expose system calls, and package TamimOS v1.0.

## Dependency Chain

- Week 1: Boot and UART for runtime diagnostics.
- Week 2: Shell as interaction layer and test harness.
- Week 3: Memory subsystem for file and process structures.
- Week 4: Scheduler for concurrent user program execution.

This week validates system-level integration quality, not just isolated features.

## Goal
Build persistent-like file operations over a virtual disk and define user-kernel service boundaries via syscalls.

Required file operations:

- create
- read
- write
- delete
- list

Required syscall capabilities:

- open
- read
- write
- exit

## What You Should Have After This

1. Implement core file system metadata for a flat namespace.
2. Design a minimal syscall interface and dispatch layer.
3. Enforce kernel-side validation for user requests.
4. Integrate shell commands with file and process primitives.
5. Package and document a release-quality OS milestone.

## Functional Requirements

1. RAM disk file system
- Initialize fixed-size RAM disk region.
- Implement flat directory table (no nested directories required).
- Track file names, sizes, and data locations.

2. File operations
- create(name)
- write(name, data)
- read(name)
- delete(name)
- list()

3. Syscall layer
- Define syscall numbers and trap/dispatch mechanism.
- Implement kernel handlers for open/read/write/exit.
- Validate pointers, lengths, and file identifiers before access.

4. User program support
- Provide at least one user program that writes and reads a file via syscalls.
- Show orderly process exit through syscall.

5. Release packaging
- Version as TamimOS v1.0.
- Include build/run instructions and architecture summary.
- Push source and documentation to GitHub repository.

## What To Build

1. Code deliverables
- File system module(s).
- Syscall entry/dispatch implementation.
- User-space sample programs or test programs.
- Updated shell commands for file manipulation.

2. Documentation deliverables
- Release notes for TamimOS v1.0.
- API-level syscall description.
- File system on-disk/in-memory layout summary.

3. Demonstration deliverables
- Transcript or screenshot sequence showing:
- boot banner
- shell prompt
- file create/write/read/list/delete
- two-process scheduling evidence (regression from Week 4)

## Suggested Shell Command Additions

- ls
- touch <name>
- cat <name>
- write <name> <text>
- rm <name>

## Validation and Test Plan

1. File operation tests
- Create multiple files with distinct names.
- Write/read variable-length payloads.
- Delete and confirm removal.
- Attempt read on deleted file and confirm error response.

2. Syscall tests
- Valid syscall execution path.
- Invalid syscall number handling.
- Invalid user pointer or oversized read/write handling.

3. Integration tests
- Run shell and scheduled user process concurrently.
- Ensure file system operations remain correct under scheduling.

## Done Criteria

- create/read/write/delete/list all work.
- Syscalls run correctly and reject invalid requests safely.
- Full flow works: boot, shell, memory, scheduler, file operations.

## Build Reflection

Include a short reflection section in your release notes:

1. Most difficult bug and how you diagnosed it.
2. One subsystem you would redesign for performance.
3. Plan for TamimOS v1.1 (for example: interrupt-driven I/O, hierarchical FS, IPC).
