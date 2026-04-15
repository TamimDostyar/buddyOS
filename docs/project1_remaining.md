# Project 1 — Remaining: IDT, ISR, and Keyboard Interrupts

## Where You Are Now

Your bootloader and kernel boot successfully. The VGA driver works — you can see text on screen. What's missing is **interrupt handling**: the mechanism that lets hardware (like the keyboard) talk to your kernel.

Right now if you press a key, nothing happens. By the end of this assignment your OS will react to keypresses and print them on screen.

---

## Background: How Interrupts Work on x86

When a key is pressed, the keyboard controller sends an electrical signal to the CPU. The CPU needs to know *what code to run* in response. This is done through three layers:

```
Keyboard press
    ↓
PIC (Programmable Interrupt Controller) — routes the signal to the CPU
    ↓
IDT (Interrupt Descriptor Table) — CPU looks up which function to call
    ↓
ISR (Interrupt Service Routine) — your handler code runs
```

### What is the IDT?

The **Interrupt Descriptor Table** is an array of 256 entries. Each entry tells the CPU: "when interrupt N fires, jump to this address." It's similar to the GDT you already set up, but for interrupts instead of memory segments.

Each IDT entry is 8 bytes:
```
Bits 0-15:   Lower 16 bits of handler address
Bits 16-31:  Kernel code segment selector (0x08 from your GDT)
Bits 32-39:  Always zero
Bits 40-47:  Flags (type, privilege level, present bit)
Bits 48-63:  Upper 16 bits of handler address
```

### What is an ISR?

An **Interrupt Service Routine** is the function the CPU jumps to when an interrupt fires. You need a small assembly stub for each interrupt that:
1. Saves all CPU registers (so your kernel state isn't corrupted)
2. Calls your C handler function
3. Restores all registers
4. Returns from the interrupt with `iret`

### What is the PIC?

The **Programmable Interrupt Controller** (8259 chip) is the hardware that routes device signals to the CPU. There are two PICs chained together:
- **Master PIC** handles IRQ 0–7 (timer, keyboard, etc.)
- **Slave PIC** handles IRQ 8–15 (real-time clock, mouse, etc.)

**Critical problem:** By default, the master PIC maps IRQ 0–7 to IDT entries 8–15. But entries 0–31 are reserved by the CPU for exceptions (divide by zero, page fault, etc.). You **must remap** the PIC so hardware IRQs don't collide with CPU exceptions.

Standard remapping:
- Master PIC → IDT entries 32–39 (IRQ 0 = entry 32, IRQ 1/keyboard = entry 33)
- Slave PIC → IDT entries 40–47

---

## Assignment Tasks

Complete the following in order. Each task builds on the previous one.

---

### Task 1: Create Port I/O Helper (`kernel/src/utils/ports.h`)

Your kernel needs to read/write to hardware I/O ports. The x86 `in` and `out` instructions do this. Create a header with two inline functions:

**`outb(uint16_t port, uint8_t val)`** — Write a byte to an I/O port:
```c
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
```

**`inb(uint16_t port)`** — Read a byte from an I/O port:
```c
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
```

These use GCC inline assembly. The `"a"` constraint means use the `al` register, `"Nd"` means use the `dx` register or an immediate. Don't worry about memorizing the syntax — just understand that `outb` sends a byte and `inb` receives one.

**Ports you'll use later:**
| Port | Purpose |
|------|---------|
| `0x20` | Master PIC command |
| `0x21` | Master PIC data |
| `0xA0` | Slave PIC command |
| `0xA1` | Slave PIC data |
| `0x60` | Keyboard data (read scancode) |

---

### Task 2: Fix the IDT Header (`kernel/src/utils/idt.h`)

Your existing `idt.h` is missing an include guard and declares a function (`process_init`) that doesn't exist. Fix it:

1. Add `#ifndef IDT_H` / `#define IDT_H` / `#endif` include guard
2. Remove the `process_init()` declaration
3. Keep the `idt_entry` and `idt_ptr` structs — they are correct
4. Keep the `idt_install()` declaration

---

### Task 3: Write the ISR Assembly Stubs (`kernel/src/utils/isr.asm`)

> **Important:** Move the existing `boot/isr.asm` to `kernel/src/utils/isr.asm`. ISR code runs in 32-bit protected mode — it belongs in the kernel, not the bootloader.

The CPU pushes different things depending on the interrupt:
- **Exceptions 8, 10–14:** CPU pushes an error code automatically
- **Everything else:** No error code — you must push a dummy `0` to keep the stack consistent

You need stubs for at least:
- ISR 0–31 (CPU exceptions)
- ISR 32–47 (hardware IRQs from the PIC)

Each stub should:
```nasm
; For interrupts WITHOUT an error code:
isr_N:
    cli
    push byte 0        ; dummy error code
    push byte N        ; interrupt number
    jmp isr_common

; For interrupts WITH an error code (8, 10, 11, 12, 13, 14):
isr_N:
    cli
    push byte N        ; interrupt number (error code already on stack)
    jmp isr_common
```

**The `isr_common` handler** must:
1. Save all registers so the C handler can inspect them
2. Set up kernel data segment
3. Call your C handler
4. Restore everything and return

```nasm
[bits 32]
extern isr_handler

isr_common:
    pusha                   ; Push edi, esi, ebp, esp, ebx, edx, ecx, eax

    ; Save segment registers
    mov ax, ds
    push eax                ; Save data segment

    ; Load kernel data segment (0x10 from your GDT)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Pass pointer to register struct on the stack
    push esp
    call isr_handler
    add esp, 4              ; Remove pushed parameter

    ; Restore segment registers
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                    ; Restore all registers
    add esp, 8              ; Remove interrupt number and error code
    sti
    iret                    ; Return from interrupt
```

**Which exceptions have error codes?**

| ISR # | Name | Error Code? |
|-------|------|-------------|
| 0 | Divide by Zero | No |
| 1 | Debug | No |
| 2 | NMI | No |
| 3 | Breakpoint | No |
| 4 | Overflow | No |
| 5 | Bound Range | No |
| 6 | Invalid Opcode | No |
| 7 | Device Not Available | No |
| 8 | Double Fault | **Yes** |
| 9 | (reserved) | No |
| 10 | Invalid TSS | **Yes** |
| 11 | Segment Not Present | **Yes** |
| 12 | Stack-Segment Fault | **Yes** |
| 13 | General Protection | **Yes** |
| 14 | Page Fault | **Yes** |
| 15–31 | Reserved/other | No |
| 32–47 | Hardware IRQs | No |

Use NASM macros (`ISR_NOERRCODE` and `ISR_ERRCODE`) like you already started — just expand them to cover all 48 entries.

---

### Task 4: Remap the PIC and Build the IDT (`kernel/src/utils/idt.c`)

Rename `idt.c.disabled` → `idt.c` and rewrite it.

#### Step A: PIC Remapping

Write a `pic_remap()` function. The PIC is programmed by sending a specific sequence of **Initialization Command Words (ICWs)** to its I/O ports. Here is the exact sequence:

```
1. Send ICW1 (0x11) to both PICs — tells them to expect initialization
     outb(0x20, 0x11)   // master command
     outb(0xA0, 0x11)   // slave command

2. Send ICW2 — the IDT offset for each PIC
     outb(0x21, 0x20)   // master: IRQs start at IDT entry 32 (0x20)
     outb(0xA1, 0x28)   // slave: IRQs start at IDT entry 40 (0x28)

3. Send ICW3 — how master/slave are wired together
     outb(0x21, 0x04)   // master: slave is on IRQ line 2 (bit 2)
     outb(0xA1, 0x02)   // slave: connected to master's IRQ 2

4. Send ICW4 (0x01) — 8086 mode
     outb(0x21, 0x01)
     outb(0xA1, 0x01)

5. Set interrupt masks — 0x00 means all IRQs enabled
     outb(0x21, 0x00)   // master: enable all
     outb(0xA1, 0x00)   // slave: enable all
```

> **Tip:** You may want to add a small I/O delay between each command. A common trick is `outb(0x80, 0)` — port 0x80 is an unused port that causes a tiny delay.

#### Step B: Register All ISR Handlers

Declare all ISR stubs as `extern`:
```c
extern void isr0(void);
extern void isr1(void);
// ... up to isr47
```

In `idt_install()`:
1. Zero out the entire `idt[256]` array
2. Call `pic_remap()`
3. Set every gate: `idt_set_gate(N, (uint32_t)isrN, 0x08, 0x8E)`
   - `0x08` = your kernel code segment selector from the GDT
   - `0x8E` = flags: present, ring 0, 32-bit interrupt gate
4. Load the IDT: `asm volatile("lidt (%0)" : : "r"(&idtp))`

---

### Task 5: Write the C Interrupt Handler (`kernel/src/utils/isr.c`)

Rename `isr.c.disabled` → `isr.c` and expand it.

The `isr_handler` function receives a pointer to the saved register state:

```c
struct regs {
    uint32_t ds;                                     // saved segment
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // from pusha
    uint32_t int_no, err_code;                       // from ISR stub
    uint32_t eip, cs, eflags, useresp, ss;           // from CPU
};
```

> **Note:** The struct layout must exactly match the order things are pushed onto the stack. If you change `isr_common` in assembly, update this struct to match.

Your handler should:

1. **For exceptions (int_no < 32):** Print the exception name and halt. Example:
   ```c
   if (r->int_no < 32) {
       vga_write("EXCEPTION: ");
       vga_write(exception_names[r->int_no]);
       vga_write("\n");
       for(;;);  // halt on exception
   }
   ```

2. **For hardware IRQs (int_no >= 32):** Dispatch to the right handler, then send EOI:
   ```c
   if (r->int_no >= 32) {
       // IRQ 1 (keyboard) = int_no 33
       if (r->int_no == 33) {
           keyboard_handler();
       }

       // Send End-Of-Interrupt to PIC
       if (r->int_no >= 40) {
           outb(0xA0, 0x20);  // EOI to slave PIC
       }
       outb(0x20, 0x20);      // EOI to master PIC
   }
   ```

   **EOI is critical.** If you don't send it, the PIC won't send any more interrupts from that IRQ line.

---

### Task 6: Implement the Keyboard Handler (`kernel/src/utils/keyboard.c`)

When the keyboard IRQ fires, a **scancode** is waiting at port `0x60`. Read it:

```c
uint8_t scancode = inb(0x60);
```

**Scancodes are NOT ASCII.** They are hardware codes. Key presses have values 0x01–0x58. Key *releases* have the same code but with bit 7 set (value ≥ 0x80). Ignore releases.

Create a lookup table to convert scancodes to characters:

```c
// US QWERTY keyboard, scancode set 1 (partial)
static const char scancode_to_ascii[128] = {
    0,   0,   '1', '2', '3', '4', '5', '6',   // 0x00-0x07
    '7', '8', '9', '0', '-', '=', '\b', '\t',  // 0x08-0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',   // 0x10-0x17
    'o', 'p', '[', ']', '\n', 0,   'a', 's',   // 0x18-0x1F
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   // 0x20-0x27
    '\'', '`', 0,  '\\', 'z', 'x', 'c', 'v',  // 0x28-0x2F
    'b', 'n', 'm', ',', '.', '/', 0,   '*',    // 0x30-0x37
    0,   ' ',  0,   0,   0,   0,   0,   0,     // 0x38-0x3F  (0x39 = space)
    // ... rest can be 0 for now
};
```

Your `keyboard_handler()`:
1. Read the scancode from port 0x60
2. Ignore key releases (scancode ≥ 0x80)
3. Look up the ASCII character
4. If it's printable, call `vga_putchar(c)`

---

### Task 7: Wire It All Together

**In `kmain.c`**, add a call to `idt_install()` before the infinite loop:

```c
#include "../utils/idt.h"

void kmain(void) {
    vga_init();
    vga_write("BuddyOS ready.\n");
    idt_install();
    for(;;);
}
```

After `idt_install()` loads the IDT and enables interrupts (`sti`), the keyboard IRQ will automatically fire whenever a key is pressed.

**In the `Makefile`**, you need to:
1. Add the new `.c` files to `KERNEL_C_SRCS` — they should already be picked up by the existing wildcard `$(wildcard $(KERNEL_SRC)/utils/*.c)`, so just make sure `idt.c`, `isr.c`, and `keyboard.c` are properly named (no `.disabled`)
2. Add a rule to assemble `isr.asm` with NASM (like the `entry.asm` rule):
   ```makefile
   $(KERNEL_SRC)/utils/isr.o: $(KERNEL_SRC)/utils/isr.asm
   	$(NASM) -f elf32 $< -o $@
   ```
3. Add `$(KERNEL_SRC)/utils/isr.o` to `KERNEL_OBJS`
4. Increase the sector count in `boot.asm` if your kernel grows beyond 4 sectors (2KB). Update the padding in the Makefile to match.

---

## Debugging Tips

- **Triple fault / reboot loop?** Your IDT is probably malformed or an entry points to garbage. Double-check that every `idt_set_gate` call uses the right function address.

- **Keyboard doesn't respond?** Make sure you:
  1. Called `idt_install()`
  2. Remapped the PIC
  3. Set IDT entry **33** (not 1) for the keyboard
  4. Send EOI (`outb(0x20, 0x20)`) at the end of the handler
  5. Interrupts are enabled — add `sti` at the end of `idt_install()`

- **Only first keypress works?** You forgot to send EOI.

- **Random exceptions firing?** Check your `isr_common` stack layout matches the `struct regs` in C exactly.

- **Use QEMU's debug output:** Run with `-d int` to log all interrupts:
  ```
  qemu-system-i386 -drive format=raw,file=build/os_image.bin -d int 2> qemu_int.log
  ```

---

## Testing Checklist

- [ ] `make os-image` builds without errors or linker warnings
- [ ] OS boots and prints startup message
- [ ] No crash/triple fault after IDT is loaded
- [ ] Pressing a key prints the correct character on screen
- [ ] Multiple keypresses work (EOI is being sent)
- [ ] Holding a key produces repeated characters
- [ ] Enter key moves to next line
- [ ] Spacebar produces a space character

## Deliverable

A bootable `os_image.bin` where BuddyOS prints a startup message and then echoes keyboard input to the VGA screen.

## Reference

- [OSDev Wiki: IDT](https://wiki.osdev.org/IDT)
- [OSDev Wiki: PIC](https://wiki.osdev.org/PIC)
- [OSDev Wiki: ISR](https://wiki.osdev.org/ISR)
- [OSDev Wiki: PS/2 Keyboard](https://wiki.osdev.org/PS/2_Keyboard)
- OSTEP textbook in `docs/operating_systems_three_easy_pieces.pdf` (Chapters on interrupts)
