#include <console.h>
#include <fs.h>
#include <heap.h>
#include <interrupt.h>
#include <kernel.h>
#include <keyboard.h>
#include <kprintf.h>
#include <multiboot.h>
#include <physical_memory.h>
#include <segmentation.h>
#include <shell.h>
#include <stdint.h>
#include <stdio.h>
#include <syscall.h>
#include <unistd.h>
#include <virtual_memory.h>

void panic(char *msg, char *file, uint32_t line) {
  kprintf("PANIC `%s` at %s:%u", msg, file, line);

  for (;;)
    asm volatile("hlt");
}

void kernel_init(multiboot_info_t *mboot_info) {
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

  kprintf("[*] initializing physical memory...");
  physical_memory_init(mboot_info);
  kprintf("done\n");

  kprintf("[*] initializing virtual memory...");
  if (!virtual_memory_init()) {
    PANIC("FAILED TO INITIALIZE VIRTUAL MEMORY");
  }
  kprintf("done\n");

  kprintf("[*] initializing file system...");
  fs_init();
  kprintf("done\n");

  interrupt_enable();
}

void show_mmap(multiboot_info_t *mboot_info) {
  kprintf("=================== memory map ===================\n");
  memory_map_t *mmap = mboot_info->mmap_addr;
  while ((uint32_t)mmap < ((uint32_t)mboot_info->mmap_addr + mboot_info->mmap_length)) {
    kprintf("base_addr=0x%x%08x, length=0x%x%08x, type=0x%x\n", mmap->base_addr_high, mmap->base_addr_low, mmap->length_high, mmap->length_low, mmap->type);
    mmap++;
  }
  kprintf("==================================================\n");
}

void kmain(multiboot_info_t *mboot_info) {
  kernel_init(mboot_info);
  show_mmap(mboot_info);

  uint32_t i = 0;
  while (true) {
    dirent *entry = fs_readdir(i);
    if (entry == NULL) {
      break;
    }

    file_t *file = fs_open(entry->name);
    if (file == NULL) {
      PANIC("FILE DOES NOT EXIST");
    }

    char buf[32];
    uint32_t size = fs_read(file, 32, buf);
    kprintf("%s = \"%s\"(%u bytes)\n", entry->name, buf, size);
    i++;
  }

  write(STDOUT, "Hello!\n", 8);
  shell_prompt();
}
