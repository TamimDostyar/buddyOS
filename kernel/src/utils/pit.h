#ifndef PIT_H
#define PIT_H

#include <stdint.h>

#define PIT_HZ 100

void     pit_init(void);
void     pit_tick(void);
uint64_t pit_ticks(void);
void     pit_sleep_ms(uint32_t ms);

#endif
