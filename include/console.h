#pragma once
#include <stdint.h>

void console_clear();

void console_putchar(char c);

void console_write(char *s);

void console_write_dec(uint32_t i);
