#pragma once
#include <multiboot.h>
#include <stdbool.h>
#include <stdint.h>

#define MEMORY_BLOCK_SIZE 0x1000

extern uint32_t __kernel_start;
extern uint32_t __kernel_end;

typedef struct {
  uint32_t system_memory_size;
  uint32_t system_memory_blocks;
  uint32_t allocated_blocks;
  uint32_t free_blocks;
  uint32_t *memory_map;
  uint32_t memory_map_size;
} physical_memory_info_t;

bool find_free_block(uint32_t *block_num);

uint32_t alloc_block();

void free_block(uint32_t physical_address);

void physical_memory_init(multiboot_info_t *mboot_info);
