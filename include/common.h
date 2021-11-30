#pragma once
#include <stdint.h>

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(cond)          \
  if (!(cond)) {              \
    PANIC("ASSERTION FAILED") \
  }

void panic(char *msg, char *file, uint32_t line);
