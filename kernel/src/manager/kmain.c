#include "../utils/vga.h"
#include "../utils/keyboard.h"

void kmain(void) {
    vga_init();
    vga_write("Mr. Tamim Dostyar!\n");
    vga_write("Kernel is running successfully!\n");

    keyboard_init();

    for (;;) {
        char c = keyboard_getchar();
        if (c != 0) {
            vga_putchar(c);
        }
    }
}