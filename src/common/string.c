#include <string.h>

uint32_t strlen(char *s) {
  uint32_t len = 0;
  while (*s) {
    s++;
    len++;
  }
  return len;
}