#pragma once
#include <stdarg.h>

int vsprintf(char *s, char *format, va_list ap);

int sprintf(char *s, char *format, ...);
