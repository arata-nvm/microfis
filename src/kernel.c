#include <common.h>
#include <console.h>
#include <interrupt.h>
#include <segmentation.h>

void kmain() {
  console_clear();

  console_write("[*] initializing segmentation...");
  segmentation_init();
  console_write("done\n");

  console_write("[*] initializing interrupt...");
  interrupt_init();
  console_write("done\n");

  asm volatile("int 3");
}
