#include <console.h>
#include <interrupt.h>
#include <io.h>
#include <isr.h>

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

#define NUM_IDT 256

idt_entry_t idt_entries[NUM_IDT];
idt_t idt;

static int_stub_t interrupt_stubs[] = {
    isr_0,
    isr_1,
    isr_2,
    isr_3,
    isr_4,
    isr_5,
    isr_6,
    isr_7,
    isr_8,
    isr_9,
    isr_10,
    isr_11,
    isr_12,
    isr_13,
    isr_14,
    isr_15,
    isr_16,
    isr_17,
    isr_18,
    isr_19,
    isr_20,
    isr_21,
    isr_22,
    isr_23,
    isr_24,
    isr_25,
    isr_26,
    isr_27,
    isr_28,
    isr_29,
    isr_30,
    isr_31,
};

static void set_idt_entry(idt_entry_t *entry, uint32_t base, uint16_t selector, uint8_t flags) {
  entry->base_low = base & 0xFFFF;
  entry->base_hi = (base >> 16) & 0xFFFF;
  entry->selector = selector;
  entry->reserved = 0;
  entry->flags = flags;
}

static void setup_idt() {
  memset(&idt_entries, 0, sizeof(idt_entry_t) * NUM_IDT);

  for (int i = INT0; i <= INT31; i++) {
    set_idt_entry(&idt_entries[i], (uint32_t)interrupt_stubs[i], 0x08, 0x8E);
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

void interrupt_dispatcher(isr_frame_t frame) {
  console_write("interrupt: ");
  console_write_dec(frame.intno);
  console_write("\n");
  PANIC("UNHANDLED EXCEPTION");
}
