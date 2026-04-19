#include "../utils/vga.h"
#include "../utils/keyboard.h"
#include "../utils/idt.h"

void kmain(void) {

    int vgaOutput = vga_init();
    int keyboardOutput = keyboard_init();
    int idtOutput = idt_install();

    if (!(keyboardOutput == 1 || idtOutput == 1 || vgaOutput == 1)){
        vga_write("Kernel failed!");
    } else {
        vga_write("Kernel is running successfully!\n");
        vga_write("To test the keyboard type characters\n");
        vga_write("\n");
    }

    // set interrupt 
    __asm__ volatile("sti");


    for (;;) {
        char c = keyboard_getchar();
        if (c != 0) {
            vga_putchar(c);
        }
    }
}
