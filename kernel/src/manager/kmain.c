#include "idt.h"
#include "keyboard.h"
#include "vga.h"
#include "heap.h"
#include "fs.h"
#include "debug.h"
#include "string.h"


extern void shell_main(void);

void kmain(void) {

  int vgaOutput = vga_init();
  int keyboardOutput = keyboard_init();
  int idtOutput = idt_install();
  // mount file system
  fs_mount();
  kprintf_str("mounting!\n");
  
  // heap
  heap_init();
  int *memory = kmalloc(sizeof(memory));
  kfree(memory);

  // set interrupt
  __asm__ volatile("sti");
  vga_write("Kernell is working and running successfully\n");
  vga_write("\n");
  vga_lock_cursor();
  
  shell_main();
  
  // fallback if shell exits
  char c = keyboard_getchar();
  while (c){
    if (system_exist != 0){
      if (c != 0){
        vga_putchar(c);
      }

    }
    break;
  }
  // for (;;) {


  // }
}
