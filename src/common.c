#include <common.h>
#include <console.h>

void panic(char *msg, char *file, uint32_t line) {
  console_write("PANIC `");
  console_write(msg);
  console_write("` at ");
  console_write(file);
  console_write(":");
  console_write_dec(line);

  for (;;)
    asm volatile("hlt");
}

void memset(void *p, uint8_t value, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    *((uint8_t *)p) = value;
    p++;
  }
}
