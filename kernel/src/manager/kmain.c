#include "idt.h"
#include "keyboard.h"
#include "vga.h"
#include "heap.h"
#include "fs.h"
#include "debug.h"
#include "string.h"
#include "session.h"
#include "pit.h"
#include "process.h"
#include "scheduler.h"
#include "syscall.h"


extern void shell_main(void);

static void read_line(const char *prompt, char *buffer, int max_len, int echo_input) {
    int index = 0;

    if (prompt != 0) {
        vga_write(prompt);
    }

    while (1) {
        char c;
        sys_read(0, &c, 1);

        if (c == '\n') {
            buffer[index] = '\0';
            vga_putchar('\n');
            return;
        }

        if (c == '\b') {
            if (index > 0) {
                index--;
                if (echo_input) {
                    vga_putchar('\b');
                    vga_putchar(' ');
                    vga_putchar('\b');
                }
            }
            continue;
        }

        if (index < max_len - 1) {
            buffer[index++] = c;
            if (echo_input) {
                vga_putchar(c);
            }
        }
    }
}

static void login_prompt(void) {
    char username[32];
    char password[32];

    while (!session_is_authenticated()) {
        vga_write("BuddyOS login\n");
        read_line("Username: ", username, sizeof(username), 1);
        read_line("Password: ", password, sizeof(password), 0);

        if (session_login(username, password)) {
            vga_write("Login successful: ");
            vga_write(session_username());
            vga_write("\n");
            return;
        }

        vga_write("Login failed. Try root / buddy or press Enter for recovery root.\n");
    }
}

void kmain(void) {
    vga_init();
    keyboard_init();
    idt_install();
    fs_mount();
    heap_init();
    pit_init();             /* requires idt_install first */
    task_system_init();

    __asm__ volatile("sti");

    vga_write("BuddyOS kernel up. Type 'help' for builtins.\n\n");
    vga_lock_cursor();
    session_init();
    login_prompt();
    shell_main();

    for (;;) __asm__ volatile("sti; hlt");
}
