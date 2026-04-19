To run this, compile first from the root with Nasm:
    - nasm -f bin boot/boot.asm -o boot/boot.bin
    - qemu-system-i386 -drive format=raw,file=boot.bin

## Goals

- Load the buddyGPT model weights into a reserved memory region at boot, before the kernel takes over
- Eventually boot directly into BuddyShell — no login prompt, no password, just `buddy>`
- Support a boot path that works without any host OS beneath it (bare metal, QEMU for development only)

---

## Architecture

The full boot sequence from power-on to the first `buddy>` prompt.

```
  Power on
      │
      ▼
  BIOS  (loads 512 bytes from disk sector 0 into 0x7C00)
      │
      ▼
  boot.asm  —  real mode  (16-bit)
      │   · prints boot message via BIOS int 0x10
      │   · reads kernel sectors from disk via int 0x13
      │   · sets up GDT
      │   · switches CPU to 32-bit protected mode
      │   · jumps to kmain()
      │
      ▼
  kmain()  —  kernel entry  (C, protected mode)
      │
      ├──► vga_init()         screen ready  (VGA text 0xB8000)
      ├──► idt_install()      interrupts ready  (IDT + PIC remap)
      ├──► keyboard_init()    input ready  (IRQ1 → keyboardHandler)
      │
      │   [FUTURE]
      ├──► kai_init()         load  /sys/kai/model.bin  into reserved memory
      │                       buddyGPT warms up, reads kernel state
      │
      └──► launch  BuddyShell  (first userspace process)
                  │
                  ▼
              buddy> _    ← user sees this; no login, no config, just the brain
```
