#include <common.h>
#include <console.h>
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

static uint32_t is_ascii_digit(char c) {
  return '0' <= c && c <= '9';
}

static uint32_t atoi(char **s) {
  uint32_t i = 0;
  while (is_ascii_digit(**s)) {
    i = i * 10 + (**s - '0');
    (*s)++;
  }
  return i;
}

void kprintf(char *format, ...) {
  va_list ap;
  va_start(ap, format);

  while (*format) {
    if (*format != '%') {
      console_putchar(*format);
      format++;
      continue;
    } else if (*(format + 1) == '%') {
      console_putchar('%');
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
    if (is_ascii_digit(*format)) {
      width = atoi(&format);
    }

    char *s;
    switch (*format) {
    case 'c':
      char buf[] = {(char)va_arg(ap, int), 0};
      s = buf;
      break;
    case 'u':
      s = utoa(va_arg(ap, uint32_t), 10);
      break;
    case 'o':
      s = utoa(va_arg(ap, uint32_t), 8);
      break;
    case 's':
      s = va_arg(ap, char *);
      break;
    case 'x':
      s = utoa(va_arg(ap, uint32_t), 16);
      break;
    default:
      console_putchar('?');
      format++;
      continue;
    }
    format++;

    uint32_t s_len = strlen(s);
    for (int i = s_len; i < width; i++) {
      console_putchar(padding_char);
    }
    console_write(s);
  }
}

void kputs(char *s) {
  console_write(s);
}
