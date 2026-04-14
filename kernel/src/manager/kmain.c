#include <stdio.h>
#include "../utils/idt.h"
#include "../utils/vga.h"


int main() {

    vga_init();
    vga_write("Kernel started\n");

    idt_install();

    asm volatile("sti");

    process_init();

    for(;;) {
        // Idle or run scheduler
    }
    return 0;
}