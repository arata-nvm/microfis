#include <common.h>
#include <console.h>
#include <stdarg.h>
#include <stdio.h>

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

void kprintf(char *format, ...) {
  va_list ap;
  va_start(ap, format);

  while (*format) {
    if (*format != '%') {
      console_putchar(*format);
      format++;
      continue;
    }

    switch (*(format + 1)) {
    case 'c':
      console_putchar(va_arg(ap, int));
      break;
    case 'u':
      console_write(utoa(va_arg(ap, uint32_t), 10));
      break;
    case 'o':
      console_write(utoa(va_arg(ap, uint32_t), 8));
      break;
    case 's':
      console_write(va_arg(ap, char *));
      break;
    case 'x':
      console_write(utoa(va_arg(ap, uint32_t), 16));
      break;
    case '%':
      console_putchar('%');
      break;
    default:
      console_putchar('?');
    }
    format += 2;
  }
}

void kputs(char *s) {
  console_write(s);
}
