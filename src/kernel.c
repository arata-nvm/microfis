#include <console.h>
#include <interrupt.h>
#include <keyboard.h>
#include <segmentation.h>
#include <stdint.h>
#include <stdio.h>

void kernel_init() {
  console_clear();

  kprintf("[*] initializing segmentation...");
  segmentation_init();
  kprintf("done\n");

  kprintf("[*] initializing interrupt...");
  interrupt_init();
  kprintf("done\n");

  kprintf("[*] initializing keyboard...");
  keyboard_init();
  kprintf("done\n");

  interrupt_enable();
}

void kmain() {
  kernel_init();

  uint16_t key;
  while (1) {
    if ((key = keyboard_getkey()) != 0) {
      console_putchar(key & KEY_CHAR_MASK);
    }
  }
}
