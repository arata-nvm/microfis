#include <io.h>

void outb(uint16_t port, uint8_t data) {
  asm volatile("out %0, %1"
               :
               : "dN"(port), "a"(data));
}

uint8_t inb(uint16_t port) {
  uint8_t data;
  asm volatile("in %0, %1"
               : "=a"(data)
               : "dN"(port));
  return data;
}
