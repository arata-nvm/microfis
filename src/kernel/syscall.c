#include <console.h>
#include <interrupt.h>
#include <kernel.h>
#include <string.h>
#include <syscall.h>

#define NUM_SYSCALL 256

static syscall_handler_t handlers[NUM_SYSCALL];

uint32_t syscall_write(uint32_t fd, uint32_t buf, uint32_t count) {
  ASSERT(fd == 1);

  uint32_t len = strlen((char *)buf);
  if (len > count) {
    len = count;
  }

  for (uint32_t i = 0; i < len; i++) {
    console_putchar(*(char *)(buf + i));
  }

  return len;
}

void syscall_dispatcher(isr_frame_t frame) {
  uint32_t no = frame.eax;
  if (handlers[no].func != 0) {
    switch (handlers[no].num_params) {
    case 3:
      frame.eax = handlers[no].func(frame.ebx, frame.ecx, frame.edx);
      break;
    }
  }
}

static void syscall_add_handler(uint32_t no, uint32_t (*func)(), uint32_t num_params) {
  handlers[no].func = func;
  handlers[no].num_params = num_params;
}

void syscall_init() {
  memset(handlers, 0, sizeof(syscall_handler_t) * NUM_SYSCALL);

  interrupt_add_handler(SYSCALL, syscall_dispatcher);

  syscall_add_handler(1, syscall_write, 3);
}