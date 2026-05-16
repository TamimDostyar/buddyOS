#include "idt.h"
#include "vga.h"
#include "keyboard.h"
#include "inb.h"
#include "isr.h"
#include "pit.h"
#include "syscall.h"
#include "scheduler.h"

#define PIC1_CMD 0x20
#define PIC2_CMD 0xA0
#define PIC_EOI  0x20

static const char *exception_messages[32] = {
    "Divide-by-zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow",
    "Bound range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 floating-point",
    "Alignment check",
    "Machine check",
    "SIMD floating-point",
    "Virtualization",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Security", "Reserved",
};

static void write_hex(uint32_t v) {
    static const char digits[] = "0123456789ABCDEF";
    char buf[11];
    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 0; i < 8; i++) {
        buf[2 + i] = digits[(v >> ((7 - i) * 4)) & 0xF];
    }
    buf[10] = '\0';
    vga_write(buf);
}

static void write_dec(uint32_t v) {
    char buf[12];
    int i = 11;
    buf[i--] = '\0';
    if (v == 0) {
        buf[i--] = '0';
    } else {
        while (v) {
            buf[i--] = '0' + (v % 10);
            v /= 10;
        }
    }
    vga_write(&buf[i + 1]);
}

void isr_handler(struct regs *r) {
    vga_write("\n\n*** KERNEL PANIC: ");
    if (r->int_no < 32) {
        vga_write(exception_messages[r->int_no]);
    } else {
        vga_write("Unknown exception");
    }
    vga_write(" (vector ");
    write_dec(r->int_no);
    vga_write(", err=");
    write_hex(r->err_code);
    vga_write(")\nEIP=");
    write_hex(r->eip);
    vga_write(" CS=");
    write_hex(r->cs);
    vga_write(" EFLAGS=");
    write_hex(r->eflags);
    vga_write("\nEAX=");
    write_hex(r->eax);
    vga_write(" EBX=");
    write_hex(r->ebx);
    vga_write(" ECX=");
    write_hex(r->ecx);
    vga_write(" EDX=");
    write_hex(r->edx);
    vga_write("\nESI=");
    write_hex(r->esi);
    vga_write(" EDI=");
    write_hex(r->edi);
    vga_write(" EBP=");
    write_hex(r->ebp);
    vga_write(" ESP=");
    write_hex(r->esp);

    if (r->int_no == 14) {
        uint32_t cr2;
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        vga_write("\nCR2=");
        write_hex(cr2);
    }
    vga_write("\n*** halted ***\n");

    for (;;) __asm__ volatile("cli; hlt");
}

void irq_handler(struct regs *r) {
    int from_timer = (r->int_no == 32);

    if (from_timer)               pit_tick();
    else if (r->int_no == 33)     keyboardHandler();

    if (r->int_no >= 40) outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);

    /* Tick-driven preemption. Safe in-IRQ: each task owns its kernel
       stack, so the saved frame above us belongs to whoever ran last. */
    if (from_timer) scheduler_on_tick();
}

/* eax=syscall, args in ebx/ecx/edx/esi; return goes back into r->eax
   so the common stub's popa restores it to the caller. */
void syscall_isr_handler(struct regs *r) {
    r->eax = (unsigned int)syscall_dispatch(
        (int)r->eax, r->ebx, r->ecx, r->edx, r->esi);
}
