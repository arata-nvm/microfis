#include <kernel.h>
#include <kprintf.h>
#include <physical_memory.h>
#include <string.h>
#include <virtual_memory.h>

page_directory_t *current_directory;

static uint32_t calc_pte_index(uint32_t virt_addr) {
  return (virt_addr >> 12) & 0x3FF;
}

static uint32_t calc_pde_index(uint32_t virt_addr) {
  return (virt_addr >> 22) & 0x3FF;
}

static page_table_entry_t *get_pte(page_table_t *table, uint32_t virt_addr) {
  if (table == NULL) {
    return NULL;
  }

  return &table->entries[calc_pte_index(virt_addr)];
}

static page_directory_entry_t *get_pde(page_directory_t *directory, uint32_t virt_addr) {
  if (directory == NULL) {
    return NULL;
  }

  return &directory->entries[calc_pde_index(virt_addr)];
}

static uint32_t calc_phys_addr(uint32_t virt_addr) {
  page_directory_entry_t *pde = get_pde(current_directory, virt_addr);
  page_table_entry_t *pte = get_pte((page_table_t *)(uint32_t)pde->table_address, virt_addr);
  return pte->frame_address | (virt_addr & 0x3FF);
}

static void write_cr3(uint32_t directory_addr) {
  asm volatile("mov cr3, %0" ::"r"(directory_addr));
}

static void flush_tlb(uint32_t virt_addr) {
  asm volatile("cli;"
               "invlpg %0;"
               "sti"
               :
               : "m"(virt_addr)
               : "memory");
}

static void enable_paging() {
  asm volatile("push eax;"
               "mov eax,cr0;"
               "or eax, 0x80000000;"
               "mov cr0, eax;"
               "pop eax");
}

void switch_directory(page_directory_t *directory) {
  if (directory != NULL) {
    write_cr3((uint32_t)directory);
    current_directory = directory;
  }
}

bool map_page(uint32_t phys_addr, uint32_t virt_addr) {
  if (current_directory == NULL) {
    return false;
  }

  page_directory_entry_t *pde = get_pde(current_directory, virt_addr);
  page_table_t *page_table;
  if (!pde->present) {
    page_table = (page_table_t *)alloc_block();
    if (page_table == NULL) {
      return false;
    }

    memset((void *)page_table, 0, 1024);

    pde->present = true;
    pde->read_write = true;
    pde->table_address = CALC_ADDR((uint32_t)page_table);
  } else {
    page_table = (page_table_t *)(uint32_t)pde->table_address;
  }

  page_table_entry_t *pte = get_pte(page_table, virt_addr);
  pte->present = true;
  pte->read_write = true;
  pte->frame_address = CALC_ADDR(phys_addr);

  return true;
}

page_table_entry_t *alloc_page(page_table_entry_t *entry) {
  uint32_t physical_address = alloc_block();
  if (physical_address == NULL) {
    return (page_table_entry_t *)physical_address;
  }

  entry->frame_address = physical_address & PTE_FRAME_ADDR_MASK;
  entry->present = true;

  return (page_table_entry_t *)entry;
}

void free_page(page_table_entry_t *entry) {
  uint32_t physical_address = entry->frame_address;
  if (physical_address == NULL) {
    return;
  }

  free_block(physical_address);
  entry->present = false;
}

void page_fault(isr_frame_t __attribute__((unused)) frame) {
  uint32_t i;
  asm volatile("mov %0, cr2"
               : "=r"(i));
  kprintf("cr2 =  0x%x\n", i);
  asm volatile("mov %0, cr3"
               : "=r"(i));
  kprintf("cr3 =  0x%x\n", i);
  kprintf("error code = 0x%x\n", frame.errno);
  PANIC("PAGE FAULT");
}

bool virtual_memory_init() {
  page_directory_t *page_directory = (page_directory_t *)alloc_block();
  if (page_directory == NULL) {
    return false;
  }

  page_table_t *page_table = (page_table_t *)alloc_block();
  if (page_table == NULL) {
    return false;
  }

  memset((void *)page_directory, 0, sizeof(page_directory_t));
  memset((void *)page_table, 0, sizeof(page_table_t));

  uint32_t addr = 0;
  for (uint32_t i = 0; i < 1024; i++) {
    page_directory->entries[i].read_write = true;

    page_table->entries[i].present = true;
    page_table->entries[i].read_write = true;
    page_table->entries[i].frame_address = CALC_ADDR(addr);

    addr += 0x1000;
  }

  page_directory->entries[0].present = true;
  page_directory->entries[0].read_write = true;
  page_directory->entries[0].table_address = CALC_ADDR((uint32_t)page_table);

  switch_directory(page_directory);
  enable_paging();

  interrupt_add_handler(INT14, page_fault);

  return true;
}
