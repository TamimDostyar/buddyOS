# Light Project 1: Bootloader and UART Bring-Up (TamimOS Week 1)

## Project Overview
This is the first milestone of TamimOS.
The goal is to bring an ARM machine from reset to kernel code and print a boot message over UART.

## Why It Matters
This project is the base for everything else.

- Project 2 uses your UART initialization for interactive shell input/output.
- Project 3 depends on your boot memory map and kernel entry boundaries.
- Project 4 uses your timer and exception setup assumptions from early boot.
- Project 5 packages all subsystems into a complete release image.

If this project is unstable, all later milestones become difficult to debug.

## Goal
Bring an ARM board from reset state to a known kernel execution point and print:

TamimOS v0.1

to a UART serial console in QEMU.

## What You Should Have After This

1. Explain reset-to-kernel control flow in a bare-metal ARM system.
2. Configure a stack pointer and memory-safe early execution region.
3. Implement memory-mapped UART output without libc or an existing runtime.
4. Build and run a flat binary or ELF image in QEMU.
5. Use disassembly and serial traces for low-level debugging.

## Functional Requirements

1. CPU startup sequence
- Provide a reset entry routine.
- Disable interrupts during early initialization (if required by your setup).
- Initialize stack pointer to a valid RAM location.

2. Control transfer
- Branch from boot entry to a C kernel entry function.
- Guarantee deterministic control flow on each boot.

3. UART initialization
- Configure UART registers for transmit mode.
- Implement blocking transmit for one byte at a time.
- Implement a string print helper using your byte transmit function.

4. Heartbeat output
- Print exactly one boot banner line containing TamimOS v0.1.
- The string must be visible in QEMU serial output.

5. Build and run workflow
- Provide reproducible commands to build and run in QEMU.
- No IDE-specific hidden configuration may be required.

## Suggested Technical Scope

- Language: C and optional ARM assembly.
- Target style: Bare-metal, no hosted OS calls.
- Toolchain: arm-none-eabi-gcc or equivalent cross toolchain.
- Emulator: QEMU system mode for ARM.

## What To Build

1. Source code
- Boot entry code (assembly or C-compatible startup file).
- Minimal kernel entry function.
- UART driver source and header.
- Linker script with clearly named sections.

2. Build artifacts
- Kernel image and/or ELF file.
- Build script or Makefile target for repeatability.

3. Verification notes
- Terminal log showing successful boot and banner output.
- Short boot-flow diagram (text or markdown is acceptable).
- 1 to 2 paragraph postmortem on at least one bug you solved.

## Implementation Guidance

1. Keep startup deterministic
- Avoid dynamic memory and large local stack objects in early boot.

2. Define memory layout explicitly
- Use a linker script to map .text, .data, .bss, stack region.

3. Keep UART code simple and testable
- Start with poll-based TX only.
- Verify single character output before full strings.

4. Fail loudly
- If your code enters an unrecoverable state, loop in a visible panic path.

## Verification Checklist

- QEMU launches with your image without immediate crash.
- PC reaches kernel entry symbol.
- Serial output includes TamimOS v0.1.
- No dependency on host libc output functions.
- Rebooting repeats the same output reliably.

## Advanced Extension (Optional)

- Add simple exception vector table stubs.
- Add UART receive polling prototype for next week.
- Add boot timestamp counter from timer peripheral if available.

## Done Criteria

- Boot reaches kernel entry reliably.
- UART prints TamimOS v0.1 in QEMU.
- Build and run steps are repeatable.
