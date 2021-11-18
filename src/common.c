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
