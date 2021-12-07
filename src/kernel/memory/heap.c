#include <heap.h>
#include <kernel.h>
#include <physical_memory.h>
#include <virtual_memory.h>

#define HEAP_SIZE (MEMORY_BLOCK_SIZE * 1024)

uint32_t heap_top = 0;
uint32_t heap_addr = 0xC0000000;
uint32_t heap_end = 0;

static void expand() {
  uint32_t top = alloc_block();
  if (!map_page(top, heap_addr)) {
    PANIC("FAILED TO ALLOC");
  }

  heap_top = heap_addr;
  heap_end = heap_addr + HEAP_SIZE - 1;
  heap_addr = heap_addr + HEAP_SIZE;
}

void *kmalloc(uint32_t size) {
  if (size > (heap_end - heap_top)) {
    expand();
  }

  void *p = (void *)heap_top;
  heap_top += size;
  heap_top = (heap_top + 7) & ~7;
  return p;
}

void kfree(void __attribute__((unused)) * p){
    // do nothing
};
