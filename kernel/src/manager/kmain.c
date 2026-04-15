#include "../utils/vga.h"

void kmain(void) {
  vga_init();
  vga_write("Mr. Tamim Dostyar!\n");
  vga_write("Kernel is running successfully!\n");

  for (;;)
    ;
}