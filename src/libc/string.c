#include <stdbool.h>
#include <string.h>

void memset(void *p, uint8_t value, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    *((uint8_t *)p) = value;
    p++;
  }
}

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

char *strcpy(char *s1, char *s2) {
  char *s = s1;
  do {
    *(s1++) = *s2;
  } while (*(s2++) != 0);
  return s;
}

char *strncpy(char *s1, char *s2, uint32_t n) {
  char *s = s1;

  while (n > 0) {
    if (*s2 == 0) {
      break;
    }

    *(s1++) = *(s2++);
    n--;
  }

  while (n > 0) {
    *(s1++) = 0;
    n--;
  }

  return s;
}

char *strtok(char *s1, char c) {
  static char *last = NULL;

  if (s1 == NULL && last == NULL) {
    return NULL;
  }

  if (last == NULL) {
    last = s1;
  }

  char *s = last;
  s1 = last;
  while (*s1 != c && *s1 != 0) {
    s1++;
  }

  if (*s1 == 0) {
    last = NULL;
  } else {
    last = s1 + 1;
  }

  *s1 = 0;

  return s;
}
