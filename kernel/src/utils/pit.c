#include "pit.h"
#include "inb.h"


#define PIT_BASE       1193182
#define PIT_CH0_DATA   0x40
#define PIT_CMD        0x43


#define PIT_CMD_INIT   0x36

static volatile uint64_t ticks_count = 0;

void pit_init(void) {
    uint32_t divisor = PIT_BASE / PIT_HZ;
    outb(PIT_CMD, PIT_CMD_INIT);
    outb(PIT_CH0_DATA, (uint8_t)(divisor & 0xFF));
    outb(PIT_CH0_DATA, (uint8_t)((divisor >> 8) & 0xFF));

    /* Unmask IRQ0 without touching IRQ1's existing mask bit. */
    uint8_t mask = inb(0x21);
    outb(0x21, mask & ~0x01);
}

void pit_tick(void) {
    ticks_count++;
}

uint64_t pit_ticks(void) {
    return ticks_count;
}

void pit_sleep_ms(uint32_t ms) {
    uint64_t target = ticks_count + (ms * PIT_HZ) / 1000;
    while (ticks_count < target) {
        __asm__ volatile("sti; hlt");
    }
}
