#pragma once

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(cond)          \
  if (!(cond)) {              \
    PANIC("ASSERTION FAILED") \
  }

void panic(char *msg, char *file, uint32_t line);

void memset(void *p, uint8_t value, uint32_t len);
