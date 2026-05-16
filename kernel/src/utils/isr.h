#ifndef ISR_H
#define ISR_H

#include <stdint.h>

struct regs {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};

void isr_handler(struct regs *r);
void irq_handler(struct regs *r);
void syscall_isr_handler(struct regs *r);

#endif
