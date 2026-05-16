#include "string.h"
#include "parser/parser.h"
#include "vga.h"
#include "keyboard.h"
#include "history/history.h"
#include "session.h"
#include "syscall.h"

static void write_prompt(void) {
    vga_write("[");
    if (session_is_root()) {
        vga_write("root");
    } else if (session_is_authenticated()) {
        vga_write(session_username());
    } else {
        vga_write("guest");
    }
    vga_write("@buddyOS] <OS>: ");
}

void shell_main(void) {
    char input[1024];

    while (1) {
        write_prompt();

        int idx = 0;
        while (1) {
            char c;
            sys_read(0, &c, 1);          /* hlt's between keys */

            if (c == '\n') {
                vga_putchar('\n');
                input[idx] = '\0';
                break;
            } else if (c == '\b') {
                if (idx > 0) {
                    idx--;
                    vga_putchar('\b');
                    vga_putchar(' ');
                    vga_putchar('\b');
                }
            } else if (idx < 1023) {
                input[idx++] = c;
                vga_putchar(c);
            }
        }

        if (input[0] != '\0') {
            store_history(input);
            parser(input);
        }
    }
}
