#include <console.h>
#include <interrupt.h>
#include <keyboard.h>
#include <multiboot.h>
#include <segmentation.h>
#include <stdint.h>
#include <stdio.h>

void kernel_init() {
  console_clear();

  kprintf("[*] initializing segmentation...");
  segmentation_init();
  kprintf("done\n");

  kprintf("[*] initializing interrupt...");
  interrupt_init();
  kprintf("done\n");

  kprintf("[*] initializing keyboard...");
  keyboard_init();
  kprintf("done\n");

  interrupt_enable();
}

void show_mmap(multiboot_info_t *mboot_info) {
  kprintf("=================== memory map ===================\n");
  memory_map_t *mmap = (memory_map_t *)mboot_info->mmap_addr;
  while ((uint32_t)mmap < (mboot_info->mmap_addr + mboot_info->mmap_length)) {
    kprintf("base_addr=0x%x%08x, length=0x%x%08x, type=0x%x\n", mmap->base_addr_high, mmap->base_addr_low, mmap->length_high, mmap->length_low, mmap->type);
    mmap++;
  }
  kprintf("==================================================\n");
}

void kmain(multiboot_info_t *mboot_info) {
  kernel_init();
  show_mmap(mboot_info);

  uint16_t key;
  while (1) {
    if ((key = keyboard_getkey()) != 0) {
      console_putchar(key & KEY_CHAR_MASK);
    }
  }
}
