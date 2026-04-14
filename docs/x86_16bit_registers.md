# BuddyOS x86 boot assembly reference

This page combines **register reference** (16-bit real mode) and a **plain-English walkthrough** of **`boot/boot.asm`**.

---

## Part 1 — Registers (16-bit real mode)

Registers are **storage inside the CPU**, not in RAM. Instructions read and write them directly. In **16-bit real mode** (typical BIOS bootloader stage), these are the names you will see most often.

### General-purpose registers (16-bit)

| Register | 8-bit parts | Typical role |
|----------|-------------|----------------|
| `ax` | `al` (low), `ah` (high) | Accumulator; arithmetic, BIOS args often in `ax` |
| `bx` | `bl`, `bh` | Base; handy for addresses and indexing |
| `cx` | `cl`, `ch` | Counter; loops, string/count operations |
| `dx` | `dl`, `dh` | Data; I/O ports (`in`/`out`), some BIOS uses |

### Pointer and index registers

| Register | Role |
|----------|------|
| `si` | Source index — often “where to read from” (`ds:si`) |
| `di` | Destination index — often “where to write to” (`es:di`) |
| `bp` | Base pointer — often frame pointer on the stack |
| `sp` | Stack pointer — current top of stack (`ss:sp`) |

### Segment registers

Segments are **not** “offsets into RAM” by themselves. With an offset (`bx`), the CPU forms a **physical address** in real mode as **(segment × 16) + offset** (same as shifting the segment left by 4 bits and adding the offset; 8086 has wrapping rules on overflow).

| Register | Name | Typical use |
|----------|------|----------------|
| `cs` | Code segment | Together with `ip`: which code runs |
| `ds` | Data segment | Default for most data reads/writes |
| `es` | Extra segment | Extra data area (string destinations) |
| `ss` | Stack segment | Together with `sp`: push/pop, `call`/`ret` |

### Instruction pointer and flags

| Name | Role |
|------|------|
| `ip` | Address of the next instruction in `cs` (not usually modified with `mov`; changed by flow control) |
| `flags` | Status and control bits; **`IF`** (interrupt flag) is cleared by `cli` and set by `sti` |

### Related instructions (next to registers)

- **`mov dest, src`** — copy a value (many combinations are legal; segment registers have restrictions).
- **`push` / `pop`** — stack operations using `ss:sp`.
- **`call` / `ret`** — subroutine call and return (save/restore return address on the stack).
- **`cli` / `sti`** — disable/enable **maskable** hardware interrupts via the `IF` flag.

**Protected mode (32-bit)** adds extended registers (`eax`, `ebx`, …) and different segmentation or paging; the bootloader switches modes after the sections below.

---

## Part 2 — `boot/boot.asm` plain-English walkthrough

### Assembler setup (top of file)

| What | How it works in plain English |
|------|-------------------------------|
| **`[bits 16]`** | Assemble instructions as **16-bit real-mode** code (default operand size 16). Matches what the CPU runs right after BIOS hands off. |
| **`[ORG 0x7C00]`** | Tell the assembler: “pretend this code is loaded at **physical address 0x7C00**.” Labels and offsets are computed as if the first byte of this file sits at that address, which is where the BIOS loads a boot sector. |
| **`[BITS 32]`** (later) | From that line onward, instructions are **32-bit protected-mode** code (operand size 32). Matches the CPU after you switch modes. |

### `start` — first instructions

| Portion | How it works in plain English |
|---------|-------------------------------|
| **`cli`** | Turn **off** maskable interrupts so nothing interrupts you while segment registers and the stack are half-configured. |
| **`xor ax, ax`** | Set **`ax` to 0** (value XOR itself is zero). |
| **`mov ds, ax` / `es` / `ss`** | Copy **0** into **data**, **extra**, and **stack** segment registers so memory accesses use a **known** segment base. |
| **`mov sp, 0x7C00`** | Put the **stack pointer** at `0x7C00`. The stack **grows downward**, so pushes use memory **below** that address—safe if you do not overwrite your own boot sector code carelessly. |
| **`sti`** | Turn **interrupts back on** now that `ss:sp` and segments are set. |
| **`mov si, boot_msg` / `call print_string`** | Point **`si`** at the **bytes of the welcome string**, then **call** the print helper. **`call`** saves the return address and jumps; **`ret`** inside `print_string` comes back here. |

### `load_kernel` — read a sector from disk (BIOS)

The BIOS uses **registers as parameters**; **`int 0x13`** is the “disk services” trap.

| Portion | How it works in plain English |
|---------|-------------------------------|
| **`mov ah, 0x02`** | Choose BIOS function **0x02** = **read sectors** from disk into RAM. |
| **`mov al, 1`** | Read **one** sector (512 bytes). Increase if the kernel spans more sectors. |
| **`ch` / `cl` / `dh`** | **Cylinder**, **sector**, **head** — old **CHS** disk addressing. Here: cylinder **0**, sector **2** (bootloader often lives in sector **1**), head **0**. |
| **`mov dl, 0x80`** | **Drive number**: `0x80` is typically the **first hard disk**; **`0x00`** is often floppy in emulators—must match how you build the image. |
| **`mov bx, 0x1000` → `mov es, bx` / `xor bx, bx`** | BIOS wants the **destination** as **`es:bx`**. Setting **`es = 0x1000`** and **`bx = 0`** means data lands at **linear address** `0x1000 × 16 + 0 = 0x10000`. (Segment × 16 + offset.) |
| **`int 0x13`** | Ask the BIOS to perform the read using all those register arguments. |
| **`jc disk_error`** | If the operation **fails**, the BIOS sets the **carry flag**. **Jump if carry** → go print an error instead of continuing. |
| **`mov si, loaded_msg` / `call print_string`** | On success, print the “kernel loaded” message. |
| **`jmp enable_a20`** | **Jump** (no return) to the next boot stage—do not fall through randomly. |

### `disk_error` and `a20_error`

| Portion | How it works in plain English |
|---------|-------------------------------|
| **`mov si, …_msg` / `call print_string`** | Same pattern: **`si`** points at a **null-terminated** string (your `db` data), print routine writes characters. |
| **`jmp halt`** | After an error, **do not** keep booting; go to the **halt** loop so the machine stops in a controlled place. |

### `enable_a20` — memory above 1 MB without 8086 wrap

| Portion | How it works in plain English |
|---------|-------------------------------|
| **`mov ax, 0x2401`** / **`int 0x15`** | BIOS function **0x2401**: **enable the A20 line** so addresses do not **wrap** at 1 MB like an old 8086. Needed for sensible protected-mode memory layout. |
| **`jc a20_error`** | If the BIOS reports failure (**carry** set), show the A20 error and **halt**. |
| **`jmp load_gdt`** | If it worked, continue to **load the GDT**. |

### `load_gdt` — tell the CPU where the table is

| Portion | How it works in plain English |
|---------|-------------------------------|
| **`lgdt [gdt_descriptor]`** | **Load GDT register** from memory at label **`gdt_descriptor`**: that memory holds **GDT size** and **GDT base address**. After this, the CPU knows where your **segment descriptors** live. |
| **`jmp switch_to_protected`** | Continue to the code that actually **enters protected mode** (could be the same label in simpler bootloaders; here it separates “load table” from “flip mode”). |

### `switch_to_protected` — enter 32-bit protected mode

| Portion | How it works in plain English |
|---------|-------------------------------|
| **`cli`** | Before changing mode, **disable interrupts** so IRQs do not run while **`cs`/IDT** are inconsistent (you have not set up protected-mode interrupts yet). |
| **`mov eax, cr0` / `or eax, 1` / `mov cr0, eax`** | Read **control register 0**, set bit **1** (**PE** = Protection Enable), write it back. That tells the CPU: **protected mode is on**. |
| **`jmp 0x08:protected_mode`** | **Far jump**: load **`cs`** with selector **`0x08`** (your **code** segment descriptor in the GDT) and jump to label **`protected_mode`**. This **flushes** prefetch and **starts** fetching **32-bit** code from the new segment. **`0x08`** = index 1 in the GDT (each entry 8 bytes; null descriptor is first). |

### Is GDT required for switching to protected mode?

Yes, for this normal bootloader flow it is required. Before setting `CR0.PE=1`, the CPU needs a valid descriptor table loaded with `lgdt`, because after switching you must use selectors (like `0x08` for code and `0x10` for data/stack) that are looked up in the GDT.

Minimal sequence:

1. Build a tiny GDT (null descriptor + code descriptor + data descriptor).
2. Load GDTR with `lgdt [gdt_descriptor]`.
3. Set `CR0.PE` to 1.
4. Do a far jump to a valid code selector (`jmp 0x08:...`).
5. Load `ds`/`es`/`ss` (and others) with valid data selectors (`0x10` here).

If you skip the GDT or use invalid selectors, the CPU faults during/after the transition (often ending in a reset/triple fault in early boot).

### `protected_mode` — 32-bit setup and run the kernel

| Portion | How it works in plain English |
|---------|-------------------------------|
| **`mov ax, 0x10`** | **`0x10`** is the **data** segment **selector** (GDT entry for flat data). |
| **`mov ds, es, fs, gs, ss, ax`** | Point all **data/stack** segments at that descriptor so reads/writes use the **same** flat layout as your code segment (typical tiny OS setup). |
| **`mov esp, 0x9C000`** | Set the **32-bit stack pointer** to a chosen high address—**below 1 MB** here so you stay in a simple RAM area before paging. |
| **`call 0x1000`** | **Call** the code at **linear address `0x1000`** as the kernel entry. **Note:** This matches “kernel at 0x1000” only if that is really where your linked kernel sits; disk load used **`0x10000`** earlier—**verify** load address and call address match your linker script. |
| **`cli` / `hlt`** | If the kernel **returns**, mask interrupts and **halt** the CPU so execution does not run into garbage. |

### `print_string` — 16-bit BIOS teletype

| Portion | How it works in plain English |
|---------|-------------------------------|
| **`lodsb`** | **Load byte** at **`ds:si`** into **`al`**, then **increment `si`**. Walks the string one character at a time. |
| **`test al, al`** | **AND** `al` with itself; sets **zero flag** if `al` is **0** (your string terminator). |
| **`jz .done`** | If we hit the **null terminator**, **jump** to **`ret`** and return to the caller. |
| **`mov ah, 0x0E`** / **`int 0x10`** | BIOS **teletype**: print **character in `al`** (often with attributes in `bx` in some variants—here minimal TTY). |
| **`jmp print_string`** | Loop until the terminator. |
| **`ret`** | Pop return address and continue after **`call print_string`**. |

### `halt` — stop forever

| Portion | How it works in plain English |
|---------|-------------------------------|
| **`cli`** | No interrupts, so **`hlt`** does not wake for IRQ handlers you did not set up. |
| **`hlt`** | **Halt** until a non-maskable event or reset; CPU idles. |
| **`jmp halt`** | If anything wakes the CPU, **halt again**—infinite loop of halt. |

### Data: messages (`db`)

| What | How it works in plain English |
|------|-------------------------------|
| **`db "...", 0x0D, 0x0A, 0`** | **`db`** = **define bytes**: raw characters. **`0x0D 0x0A`** = **CR/LF** (new line on many displays). Final **`0`** = **C-style null terminator** so `lodsb` + `test` knows where to stop. |

### GDT — `gdt_start`, descriptors, `gdt_descriptor`

| What | How it works in plain English |
|------|-------------------------------|
| **`gdt_start:` / `dq 0x...`** | **`dq`** = **8 bytes**. First entry is the **mandatory null descriptor** (all zeros). |
| **`gdt_code` / `gdt_data`** | Each **segment descriptor** is **64 bits** split across **`dw`/`db`** fields: **limit**, **base**, **access** (code vs data, ring, present), **granularity** (4K units, 32-bit). Together they describe **flat** segments covering the low 4 GB in a typical minimal GDT. |
| **`gdt_descriptor`** | **`dw`** size = **total GDT length − 1** (per Intel spec). **`dd`** = **32-bit linear address** of **`gdt_start`** (fine in real mode for `lgdt` if address fits and setup is consistent). |
| **`lgdt`** (again) | CPU uses this structure to interpret selectors like **`0x08`** and **`0x10`**. |

### Boot sector padding and signature

| What | How it works in plain English |
|------|-------------------------------|
| **`times 510 - ($ - $$) db 0`** | **Pad** the image so the total size up to this point is **510 bytes**: `$` = current position, `$$` = start of section—so **`$ - $$`** = bytes emitted so far. BIOS expects **512-byte** sector with **last two bytes** = signature. |
| **`dw 0xAA55`** | **Boot signature** in **little-endian** (`55 AA` on disk). BIOS only treats the sector as bootable if these two bytes are at the end. |

---

## Part 3 — Quick instruction index

| Instruction | Plain English |
|-------------|----------------|
| **`mov`** | Copy / assign: destination gets the same value as source (source unchanged for register sources). |
| **`xor r, r`** | Zero a register by XOR with itself. |
| **`cli` / `sti`** | Clear / set **interrupt flag** (maskable IRQs off/on). |
| **`int n`** | Software interrupt: call BIOS (or handler) for vector **`n`**. |
| **`jc`** | Jump if **carry** (often “BIOS reported error”). |
| **`jmp`** | Goto; **`jmp label`** does not return. |
| **`call` / `ret`** | Call subroutine / return to saved address. |
| **`lgdt [mem]`** | Load **GDTR** from 6 bytes at `mem` (limit + base). |
| **`or eax, 1`** | Set **PE** bit in `cr0` (among other bits if they were already set). |
| **`lodsb`** | Load byte from `[ds:si]` into `al`, increment `si`. |
| **`test a, a`** | Bitwise AND; sets **zero** flag if result is 0. |
| **`jz`** | Jump if **zero** flag set. |
| **`hlt`** | Halt until interrupt or reset. |


refrence: [VIRGINIA_TECH](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)