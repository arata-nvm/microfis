#include <string.h>

uint32_t strlen(char *s) {
  uint32_t len = 0;
  while (*s) {
    s++;
    len++;
  }
  return len;
}

int32_t strcmp(char *s1, char *s2) {
  while (*s1 && *s1 - *s2 == 0) {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}
