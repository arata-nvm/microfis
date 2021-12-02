#include <console.h>
#include <interrupt.h>
#include <keyboard.h>
#include <kprintf.h>
#include <multiboot.h>
#include <segmentation.h>
#include <shell.h>
#include <stdint.h>
#include <stdio.h>
#include <syscall.h>
#include <unistd.h>

void panic(char *msg, char *file, uint32_t line) {
  kprintf("PANIC `%s` at %s:%u", msg, file, line);

  for (;;)
    asm volatile("hlt");
}

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

  kprintf("[*] initializing syscall...");
  syscall_init();
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
  write(STDOUT, "Hello!\n", 8);
  shell_prompt();
}
