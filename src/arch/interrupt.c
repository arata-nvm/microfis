#include <interrupt.h>
#include <io.h>
#include <isr.h>
#include <stdint.h>
#include <string.h>

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

#define NUM_IDT 256

idt_entry_t idt_entries[NUM_IDT];
idt_t idt;

static isr_handler_t isr_handlers[NUM_IDT];

static isr_stub_t isr_stubs[] = {
    isr_0, isr_1, isr_2, isr_3, isr_4, isr_5, isr_6, isr_7, isr_8, isr_9,
    isr_10, isr_11, isr_12, isr_13, isr_14, isr_15, isr_16, isr_17, isr_18, isr_19,
    isr_20, isr_21, isr_22, isr_23, isr_24, isr_25, isr_26, isr_27, isr_28, isr_29,
    isr_30, isr_31, isr_32, isr_33, isr_34, isr_35, isr_36, isr_37, isr_38, isr_39,
    isr_40, isr_41, isr_42, isr_43, isr_44, isr_45, isr_46, isr_47};

static void set_idt_entry(idt_entry_t *entry, uint32_t base, uint16_t selector, uint8_t flags) {
  entry->base_low = base & 0xFFFF;
  entry->base_hi = (base >> 16) & 0xFFFF;
  entry->selector = selector;
  entry->reserved = 0;
  entry->flags = flags;
}

static void setup_idt() {
  memset(&idt_entries, 0, sizeof(idt_entry_t) * NUM_IDT);
  memset(&isr_handlers, 0, sizeof(isr_handler_t) * NUM_IDT);

  for (int i = INT0; i <= INT31; i++) {
    set_idt_entry(&idt_entries[i], (uint32_t)isr_stubs[i], 0x08, 0x8E);
  }
  for (int i = IRQ0; i <= IRQ15; i++) {
    set_idt_entry(&idt_entries[i], (uint32_t)isr_stubs[i], 0x08, 0x8E);
  }
}

static void load_idt() {
  idt.base = idt_entries;
  idt.limit = sizeof(idt_entry_t) * NUM_IDT - 1;

  asm volatile("lidt idt");
}

static void remap_pic() {
  // start initialization
  outb(PIC1_CMD, 0x11);
  outb(PIC2_CMD, 0x11);

  // PIC vector offset
  outb(PIC1_DATA, 0x20); // IRQ0-IRQ7  -> 32-39
  outb(PIC2_DATA, 0x28); // IRQ8-IRQ15 -> 40-47

  // how PICs are connected
  outb(PIC1_DATA, 0x4);
  outb(PIC2_DATA, 0x2);

  // x86 mode
  outb(PIC1_DATA, 0x1);
  outb(PIC2_DATA, 0x1);

  // masks
  outb(PIC1_DATA, 0x0);
  outb(PIC2_DATA, 0x0);
}

void interrupt_init() {
  remap_pic();
  setup_idt();
  load_idt();
}

void interrupt_disable() {
  asm volatile("cli");
}

void interrupt_enable() {
  asm volatile("sti");
}

void interrupt_add_handler(uint8_t intno, isr_handler_t handler) {
  isr_handlers[intno] = handler;
}

void interrupt_dispatcher(isr_frame_t frame) {
  // notify EOI
  if (IRQ0 <= frame.intno && frame.intno <= IRQ15) {
    if (frame.intno >= IRQ8) {
      outb(PIC2_CMD, 0x20);
    }
    outb(PIC1_CMD, 0x20);
  }

  if (isr_handlers[frame.intno] != 0) {
    isr_handlers[frame.intno](frame);
  }
}
