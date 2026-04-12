; BuddyOS — minimal 32-bit x86 bring-up stub (Multiboot v1).
; For a future linked kernel booted by GRUB or qemu -kernel.
; Other parts of this repo (e.g. src/shell) are normal programs for Linux/macOS.

[BITS 32]

MBOOT_MAGIC equ 0x1BADB002
MBOOT_FLAGS equ 0x00000003
MBOOT_CHECK equ -(MBOOT_MAGIC + MBOOT_FLAGS)

ALIGN 4
multiboot_header:
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECK

global start
start:
    mov esp, _sys_stack
hang:
    jmp hang

SECTION .bss
ALIGN 16
    resb 8192
_sys_stack:
