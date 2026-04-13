# Bootloader Status and Runbook

This file documents the current BIOS bootloader state and the exact terminal flow used to verify progress.

## Current state

- `boot.asm` is a BIOS-compatible 16-bit boot sector (`ORG 0x7C00`).
- It prints `BuddyOS booted!` using BIOS teletype (`int 0x10`).
- It pads to 512 bytes and ends with boot signature `0xAA55`.
- `Makefile` includes:
  - `make boot` to assemble `build/boot.bin`
  - `make run-boot` to run with QEMU (`qemu-system-i386`)

## What happened (recorded)

From terminal session:

1. Installed NASM:
   - `brew install nasm`
2. Built boot image:
   - `make boot`
3. Build succeeded:
   - `nasm -f bin boot.asm -o build/boot.bin`

## Validation checklist

Run these commands to confirm the boot sector is valid:

```bash
ls -lh build/boot.bin
xxd -s 510 -l 2 build/boot.bin
```

Expected:

- `build/boot.bin` size is exactly `512` bytes
- Last two bytes are `55aa`

## Boot test

```bash
make run-boot
```

Expected behavior:

- QEMU starts
- Bootloader prints `BuddyOS booted!`
- CPU halts in a loop (intentional for this stage)

## If tools are missing

Install locally on macOS:

```bash
brew install nasm qemu
```

## Next milestone

Move from "message + halt" to loading a second-stage kernel:

1. Read sectors from disk (`int 0x13`)
2. Jump to loaded kernel entry
3. Implement minimal `kmain` loop
