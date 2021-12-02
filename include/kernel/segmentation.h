#pragma once
#include <stdint.h>

typedef struct {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t flags1;
  uint8_t limit_hi : 4;
  uint8_t flags2 : 4;
  uint8_t base_hi;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
  uint16_t limit;
  gdt_entry_t *base;
} __attribute__((packed)) gdt_t;

void segmentation_init();
