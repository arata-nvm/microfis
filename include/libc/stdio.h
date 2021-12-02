#pragma once
#include <stdarg.h>

#define STDOUT 1

int vsprintf(char *s, char *format, va_list ap);

int sprintf(char *s, char *format, ...);
