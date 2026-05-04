// #include <stdio.h>
#include "string.h"
#include "parser/parser.h"
#include "vga.h"
#include "keyboard.h"

// #include "history.h"

void shell_main(void) {
    char input[1024];
    
    // condition is true 1 = true
    while (1) {
        vga_write("<OS>: ");
        
        int idx = 0;
        while (1) {
            char c = keyboard_getchar();
            if (c != 0) {
                if (c == '\n') {
                    vga_putchar('\n');
                    input[idx] = '\0';
                    break;
                } else if (c == '\b') { // simple backspace
                    if (idx > 0) {
                        idx--;
                        vga_putchar('\b');
                        vga_putchar(' ');
                        vga_putchar('\b');
                    }
                } else {
                    if (idx < 1023) {
                        input[idx++] = c;
                        vga_putchar(c);
                    }
                }
            }
        }
        
        if (strlen(input) > 0) {
            parser(input);
        }
    }
}