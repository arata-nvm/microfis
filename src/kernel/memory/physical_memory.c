#include <kprintf.h>
#include <physical_memory.h>
#include <string.h>

physical_memory_info_t pm_info;

uint32_t kernel_size() {
  return (uint32_t)(&__kernel_end - &__kernel_start);
}

static void set_allocated(uint32_t bit_num) {
  pm_info.memory_map[bit_num / 32] |= (1 << (bit_num % 32));
}

static void set_free(uint32_t bit_num) {
  pm_info.memory_map[bit_num / 32] &= ~(1 << (bit_num % 32));
}

static bool is_allocated(uint32_t bit_num) {
  return (pm_info.memory_map[bit_num / 32] & (1 << (bit_num % 32))) != 0;
}

bool find_free_block(uint32_t *block_num) {
  for (uint32_t index = 0; index < pm_info.memory_map_size; index++) {
    if (pm_info.memory_map[index] == 0xFFFFFFFF) {
      continue;
    }

    for (uint32_t bit_num = 0; bit_num < 32; bit_num++) {
      if ((pm_info.memory_map[index] & (1 << bit_num)) == 0) {
        *block_num = index * sizeof(uint32_t) * 8 + bit_num;
        return true;
      }
    }
  }

  return false;
}

static void alloc_region(uint32_t base_addr, uint32_t size) {
  uint32_t block_num = base_addr / MEMORY_BLOCK_SIZE;
  uint32_t block_size = size / MEMORY_BLOCK_SIZE;

  for (uint32_t i = 0; i < block_size; i++) {
    set_allocated(block_num);

    block_num--;
    pm_info.allocated_blocks++;
    pm_info.free_blocks--;
  }
}

static void free_region(uint32_t base_addr, uint32_t size) {
  uint32_t block_num = base_addr / MEMORY_BLOCK_SIZE;
  uint32_t block_size = size / MEMORY_BLOCK_SIZE;

  for (uint32_t i = 0; i < block_size; i++) {
    set_free(block_num);

    block_num--;
    pm_info.allocated_blocks--;
    pm_info.free_blocks++;
  }
}

uint32_t alloc_block() {
  if (pm_info.free_blocks <= 0) {
    return NULL;
  }

  uint32_t block_num;
  if (!find_free_block(&block_num)) {
    return NULL;
  }

  set_allocated(block_num);
  uint32_t physical_address = block_num * MEMORY_BLOCK_SIZE;
  pm_info.allocated_blocks++;
  pm_info.free_blocks--;

  return physical_address;
}

void free_block(uint32_t physical_address) {
  uint32_t block_num = physical_address / MEMORY_BLOCK_SIZE;
  set_free(block_num);
  pm_info.allocated_blocks--;
  pm_info.free_blocks++;
}

void init_memory(uint32_t memory_size) {
  pm_info.system_memory_size = memory_size;
  pm_info.system_memory_blocks = memory_size / MEMORY_BLOCK_SIZE;
  pm_info.allocated_blocks = pm_info.system_memory_blocks;
  pm_info.free_blocks = 0;
  pm_info.memory_map = (uint32_t *)(__kernel_end);
  pm_info.memory_map_size = pm_info.system_memory_blocks / 32;

  memset(pm_info.memory_map, 0xFF, pm_info.memory_map_size);
}

void physical_memory_init(multiboot_info_t *mboot_info) {
  uint32_t total_memory_kb = (mboot_info->mem_upper << 8) | mboot_info->mem_lower;
  init_memory(total_memory_kb * 1024);

  memory_map_t *mmap = mboot_info->mmap_addr;
  for (; mmap < (mboot_info->mmap_addr + mboot_info->mmap_length); mmap++) {
    uint32_t send_addr = (mmap->base_addr_high << 8) | mmap->base_addr_low;
    uint32_t send_length = (mmap->length_high << 8) | mmap->length_low;

    if (mmap->type == MBOOT_MEMORY_AVAILABLE || mmap->type == MBOOT_MEMORY_ACPI_RECLAIMABLE) {
      if (send_addr == (uint32_t)&__kernel_start) {
        alloc_region((uint32_t)send_addr, kernel_size() + pm_info.memory_map_size);
        free_region((uint32_t)&__kernel_end, send_length - (kernel_size() + pm_info.memory_map_size));
      } else {
        free_region(send_addr, send_length);
      }
    } else {
      alloc_region(send_addr, send_length);
    }
  }
}