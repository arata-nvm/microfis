#include <console.h>
#include <ctype.h>
#include <kernel.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char *utoa(uint32_t i, uint32_t radix) {
  ASSERT(radix == 8 || radix == 10 || radix == 16);

  const char *hex = "0123456789ABCDEF";

  static char buf[32];

  char *buf_ptr = buf + sizeof(buf);
  *(--buf_ptr) = 0;
  do {
    *(--buf_ptr) = hex[i % radix];
    i /= radix;
  } while (i != 0);

  return buf_ptr;
}

static uint32_t atoi(char **s) {
  uint32_t i = 0;
  while (isdigit(**s)) {
    i = i * 10 + (**s - '0');
    (*s)++;
  }
  return i;
}

int vsprintf(char *s, char *format, va_list ap) {
  char *s_orig = s;

  while (*format) {
    if (*format != '%') {
      *(s++) = *format;
      format++;
      continue;
    } else if (*(format + 1) == '%') {
      *(s++) = '%';
      format += 2;
      continue;
    }

    format++;

    char padding_char = ' ';
    if (*format == '0') {
      padding_char = '0';
      format++;
    }

    int width = -1;
    if (isdigit(*format)) {
      width = atoi(&format);
    }

    char *buf;
    switch (*format) {
    case 'c':
      char c[] = {(char)va_arg(ap, int), 0};
      buf = c;
      break;
    case 'u':
      buf = utoa(va_arg(ap, uint32_t), 10);
      break;
    case 'o':
      buf = utoa(va_arg(ap, uint32_t), 8);
      break;
    case 's':
      buf = va_arg(ap, char *);
      break;
    case 'x':
      buf = utoa(va_arg(ap, uint32_t), 16);
      break;
    default:
      *(s++) = '?';
      format++;
      continue;
    }
    format++;

    uint32_t buf_len = strlen(buf);
    for (int i = buf_len; i < width; i++) {
      *(s++) = padding_char;
    }
    strcpy(s, buf);
    s += buf_len;
  }

  *s = 0;
  return strlen(s_orig);
}

int sprintf(char *s, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  int i = vsprintf(s, format, ap);
  va_end(ap);

  return i;
}
