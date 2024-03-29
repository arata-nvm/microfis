#pragma once
#include <stdint.h>

void memset(void *p, uint8_t value, uint32_t len);

uint32_t strlen(char *s);

int32_t strcmp(char *s1, char *s2);

char *strcpy(char *s1, char *s2);

char *strncpy(char *s1, char *s2, uint32_t n);

char *strtok(char *s1, char c);
