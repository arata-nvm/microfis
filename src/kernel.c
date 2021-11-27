#include <common.h>
#include <console.h>
#include <interrupt.h>
#include <segmentation.h>

void timer_interrupt_handler(isr_frame_t __attribute__((unused)) frame) {
  console_write(".");
}

void kmain() {
  console_clear();

  console_write("[*] initializing segmentation...");
  segmentation_init();
  console_write("done\n");

  console_write("[*] initializing interrupt...");
  interrupt_init();
  interrupt_add_handler(32, timer_interrupt_handler);
  console_write("done\n");

  asm volatile("sti");
}
