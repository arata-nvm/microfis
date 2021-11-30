#include <common.h>
#include <stdio.h>

void panic(char *msg, char *file, uint32_t line) {
  kprintf("PANIC `%s` at %s:%u", msg, file, line);

  for (;;)
    asm volatile("hlt");
}

void memset(void *p, uint8_t value, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    *((uint8_t *)p) = value;
    p++;
  }
}
