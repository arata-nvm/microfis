#pragma once
#include <stdint.h>

typedef struct {
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t addr;
  uint32_t reserved;
} aout_symbol_table_t;

typedef struct {
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
} elf_section_header_table_t;

typedef struct {
  uint32_t size;
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t length_low;
  uint32_t length_high;
  uint32_t type;
} __attribute__((packed)) memory_map_t;

#define MBOOT_MEMORY_AVAILABLE 1
#define MBOOT_MEMORY_RESERVED 2
#define MBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MBOOT_MEMORY_NVS 4
#define MBOOT_MEMORY_BADRAM 5

typedef struct {
  uint32_t flags;

  uint32_t mem_lower;
  uint32_t mem_upper;

  uint32_t boot_device;

  uint32_t cmdline;

  uint32_t mods_count;
  uint32_t mods_addr;

  union {
    aout_symbol_table_t aout_sym;
    elf_section_header_table_t elf_sec;
  } u;

  uint32_t mmap_length;
  uint32_t mmap_addr;
} multiboot_info_t;