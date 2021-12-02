#include <console.h>
#include <kernel.h>
#include <kprintf.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void kprintf(char *format, ...) {
  va_list ap;
  char buf[1024];

  va_start(ap, format);
  vsprintf(buf, format, ap);
  va_end(ap);

  console_write(buf);
}

void kputs(char *s) {
  console_write(s);
}
