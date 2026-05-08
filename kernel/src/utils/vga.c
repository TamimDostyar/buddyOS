#include "vga.h"
#include "inb.h"
#include "debug.h"

int system_exist = 1;

// 0xB8000 physcall memory address for the vga
static uint16_t * const VGA_MEMORY = (uint16_t*)0xB8000;
static int cursor_row = 0;
static int cursor_col = 0;
static uint8_t vga_current_color = 0;
static int protected_row = 0;
static int protected_col = 0;

static inline uint8_t vga_make_color(uint8_t fg, uint8_t bg) {
    return (bg << 4) | (fg & 0x0F);
}


void vga_set_cursor_pos(int col, int row) {
    uint16_t pos = row * VGA_WIDTH + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void quit_terminal(void){
    system_exist = 0;
    outb(0x501, 0x00);
    kprintf_int(system_exist);
    kprintf_str("\n");
}

// void disable_blinker(){
//     //select the port
//     outb(0x3D4, 0x0A);
//     // Set bit 5 (cursor disable bit)
//     outb(0x3D5, 0x20);
// }


//helper get current 16-bit value for a character
static inline uint16_t vga_entry(char c, uint8_t color) {
    return ((uint16_t)color << 8) | (uint8_t)c;
}


int vga_init(void) {
    vga_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    vga_clear();
    return 1;
}


void vga_clear() {
    for (int row = 0; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            VGA_MEMORY[row * VGA_WIDTH + col] = vga_entry(' ', vga_current_color);
        }
    }
    cursor_row = 0;
    cursor_col = 0;
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    vga_current_color = vga_make_color(fg, bg);
}

static void vga_scroll() {
    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            VGA_MEMORY[(row - 1) * VGA_WIDTH + col] = VGA_MEMORY[row * VGA_WIDTH + col];
        }
    }
    for (int col = 0; col < VGA_WIDTH; col++) {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = vga_entry(' ', vga_current_color);
    }
    cursor_row = VGA_HEIGHT - 1;
    cursor_col = 0;
}

void vga_putchar(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
        // just protect to go again in the above row.
        protected_row = cursor_row;
        protected_col = cursor_col;

    } else if(c == '\b'){
        int next_col = cursor_col - 1;
        int next_row = cursor_row;

        if (next_col < 0) {
            next_row--;
            next_col = VGA_WIDTH - 1;
        }

        if (next_row < protected_row || (next_row == protected_row && next_col < protected_col)) {
            return;
        }

        cursor_col = next_col;
        cursor_row = next_row;
        VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(' ', vga_current_color);

    }
    
    else {
        VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(c, vga_current_color);
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    if (cursor_row >= VGA_HEIGHT) {
        vga_scroll();
    }
    vga_set_cursor_pos(cursor_col, cursor_row);
}

void vga_lock_cursor(void) {
    protected_row = cursor_row;
    protected_col = cursor_col;
}

// write in the position of the cursor
void vga_write(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i]);
    }
}
