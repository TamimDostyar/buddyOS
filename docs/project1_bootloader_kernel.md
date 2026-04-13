# Project 1: Bootloader and Minimal C Kernel

## Prerequisites
None. This is the foundation of BuddyOS.

## Goal
Build the first bootable version of BuddyOS: a bootloader that loads your kernel, enters protected mode, and starts a minimal C kernel that can print text and react to keyboard input.

## What You Will Build
- 16-bit bootloader assembly that loads the kernel from disk into memory.
- Protected mode setup (GDT, transition code, and jump to kernel entry).
- Minimal C kernel runtime and entry point.
- VGA text output driver for status messages.
- Basic interrupt setup for keyboard input.

## Required Features
1. Boot flow
- BIOS loads your boot sector.
- Bootloader reads kernel sectors from disk image.
- Control transfers cleanly to kernel entry.

2. Protected mode transition
- Define and load a valid GDT.
- Enable protected mode and verify execution continues in 32-bit code.

3. Minimal kernel services
- Initialize screen output and print startup text.
- Configure IDT entries needed for keyboard handling.
- Handle keyboard interrupt and display typed keys.

4. Reliability
- If any stage fails, print a simple diagnostic message.
- Keep system alive in a loop after initialization.

## Suggested Structure
- `boot/bootloader.asm`
- `boot/gdt.asm`
- `kernel/entry.asm`
- `kernel/main.c`
- `kernel/vga.c`
- `kernel/interrupts.c`
- `kernel/keyboard.c`

## Testing Checklist
- Disk image boots in QEMU without host OS dependencies.
- Kernel prints `BuddyOS ready` on startup.
- Keyboard input appears on screen.
- Rebooting preserves expected behavior.

## Deliverable
A bootable `os-image.bin` that starts BuddyOS in QEMU, prints `BuddyOS ready`, and shows keypresses on screen.

## Next Step
Proceed to Project 2: Kernel Memory Allocator.
