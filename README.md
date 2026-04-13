# BuddyOS
<p align="center">
  <img src="asset/cIcon.png" width="32%" />
</p>

Educational implementations of core operating-system concepts in **C** for **BuddyOS**—an OS built from scratch. The repository is organized as milestones (boot path, shell, memory management, concurrency, file system, and kernel interfaces). Each milestone targets BuddyOS behavior; a minimal **cross-compiler / emulator** setup (for example QEMU) is only a development convenience, not a dependency on a full desktop OS as the runtime.

Optional use of **C++** is limited to assignments that explicitly require it.

## Objectives

- Implement and exercise the same ideas covered in typical OS coursework: execution, heap layout, synchronization, on-disk structure, and the user–kernel boundary—**on BuddyOS**.
- Produce code that can be built, tested, and demonstrated per milestone as BuddyOS grows.
- Keep specifications, checklists, and notes alongside the code in version control.

## Scope

Milestones are defined for **BuddyOS**: the shell runs in the BuddyOS environment and launches programs via BuddyOS process APIs; the allocator uses the BuddyOS heap or equivalent kernel interface; threading is user-level on BuddyOS; the file system attaches to BuddyOS block/VFS layers; kernel work adds BuddyOS syscalls and in-kernel facilities—not third-party kernel module frameworks.

## Milestones

| Order | Focus | Summary |
|------:|--------|---------|
| 1 | Shell | Parser, BuddyOS process launch, built-ins, pipes, I/O redirection |
| 2 | Allocator | `malloc` family semantics, free list, coalescing, alignment |
| 3 | Concurrency | User-level threads, mutexes, condition variables, concurrent data structures |
| 4 | File system | Virtual disk, block bitmap, inodes, directories—BuddyOS integration |
| 5 | Kernel | Custom BuddyOS syscall, kernel info path, user-mode test |

Detailed requirements and acceptance criteria for each milestone are under `docs/`.

## Repository layout

```
docs/          Assignment specifications and build notes
shell/         Interactive shell (milestone 1)
alloc/         Memory allocator (milestone 2; placeholder until implemented)
thread/        Threads and synchronization (milestone 3)
fs/            File-system implementation (milestone 4)
kernel/        Kernel and syscall-related sources (milestone 5)
```

## Prerequisites

Typical expectations per milestone include:

- **Toolchain:** A C toolchain that can produce BuddyOS binaries (GCC/Clang with appropriate flags, `make`, and NASM where assembly is used).
- **Bring-up:** QEMU (or similar) to run the boot image and later kernels, as documented in `OSdocumentation/bootloader-status.md`.
- **Milestone 4:** BuddyOS block device or RAM-disk path to exercise the on-disk layout.
- **Milestone 5:** Ability to rebuild the BuddyOS kernel and run user-mode tests that invoke new syscalls.

Exact versions and steps are specified in the assignment documents under `docs/`.

## Building and running

There is no single top-level build for every milestone yet; each milestone is built and invoked according to its spec. Boot sector build and QEMU steps are in `OSdocumentation/bootloader-status.md`. The shell under `shell/` may include a local README with run instructions.

## Documentation

Authoritative milestone text, testing checklists, and suggested source layouts live in **`docs/`**.

- Bootloader progress and reproducible run steps: **`OSdocumentation/bootloader-status.md`**.


This project will eventually be used to run BuddyAI, which is currently under development. You can find the BuddyAI repository here: [REPO FOR BUDDY AI](https://github.com/TamimDostyar/buddy/)
