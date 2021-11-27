#include <common.h>
#include <console.h>
#include <interrupt.h>
#include <keyboard.h>
#include <segmentation.h>

void kmain() {
  console_clear();

  console_write("[*] initializing segmentation...");
  segmentation_init();
  console_write("done\n");

  console_write("[*] initializing interrupt...");
  interrupt_init();
  console_write("done\n");

  console_write("[*] initializing keyboard...");
  keyboard_init();
  console_write("done\n");

  interrupt_enable();

  uint16_t key;
  while (1) {
    if ((key = keyboard_getkey()) != 0) {
      console_putchar(key & KEY_CHAR_MASK);
    }
  }
}
