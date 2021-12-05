#pragma once
#include <stdint.h>

#define PTE_FRAME_ADDR_MASK 0xFFFFF000
#define CALC_ADDR(addr) ((addr) >> 12)

typedef struct {
  uint32_t present : 1;
  uint32_t read_write : 1;
  uint32_t us : 1;
  uint32_t pwt : 1;
  uint32_t pcd : 1;
  uint32_t accessed : 1;
  uint32_t dirty : 1;
  uint32_t pat : 1;
  uint32_t global : 1;

  uint32_t ignored : 3;
  uint32_t frame_address : 20;
} __attribute__((packed)) page_table_entry_t;

typedef struct {
  page_table_entry_t entries[1024];
} page_table_t;

typedef struct {
  uint32_t present : 1;
  uint32_t read_write : 1;
  uint32_t us : 1;
  uint32_t pwt : 1;
  uint32_t pcd : 1;
  uint32_t accessed : 1;
  uint32_t reserved : 1;
  uint32_t ps : 1;
  uint32_t global : 1;

  uint32_t ignored : 3;
  uint32_t table_address : 20;
} page_directory_entry_t;

typedef struct {
  page_directory_entry_t entries[1024];
} page_directory_t;

page_table_entry_t *alloc_page(page_table_entry_t *entry);

void free_page(page_table_entry_t *entry);

bool virtual_memory_init();
