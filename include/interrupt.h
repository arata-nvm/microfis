#pragma once
#include <common.h>

typedef struct {
  uint16_t base_low;
  uint16_t selector;
  uint8_t reserved;
  uint8_t flags;
  uint16_t base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct {
  uint16_t limit;
  idt_entry_t *base;
} __attribute__((packed)) idt_t;

typedef struct {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t intno, errno;
  uint32_t eip, cs, eflags, prev_esp, ss;
} isr_frame_t;

typedef void (*int_stub_t)();

enum {
  INT0,
  INT1,
  INT2,
  INT3,
  INT4,
  INT5,
  INT6,
  INT7,
  INT8,
  INT9,
  INT10,
  INT11,
  INT12,
  INT13,
  INT14,
  INT15,
  INT16,
  INT17,
  INT18,
  INT19,
  INT20,
  INT21,
  INT22,
  INT23,
  INT24,
  INT25,
  INT26,
  INT27,
  INT28,
  INT29,
  INT30,
  INT31,
  IRQ0,
  IRQ1,
  IRQ2,
  IRQ3,
  IRQ4,
  IRQ5,
  IRQ6,
  IRQ7,
  IRQ8,
  IRQ9,
  IRQ10,
  IRQ11,
  IRQ12,
  IRQ13,
  IRQ14,
  IRQ15,
};

void interrupt_init();

void interrupt_dispatcher(isr_frame_t frame);
