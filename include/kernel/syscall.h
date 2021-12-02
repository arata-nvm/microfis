#pragma once
#include <interrupt.h>

typedef struct {
  uint32_t (*func)();
  uint32_t num_params;
} syscall_handler_t;

void syscall_dispatcher(isr_frame_t frame);

void syscall_init();
