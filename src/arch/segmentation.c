#include <segmentation.h>

#define NUM_SEGMENT 5

static gdt_entry_t gdt_entries[NUM_SEGMENT];
static gdt_t gdt;

static void set_gdt_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit, uint16_t flags) {
  entry->base_low = base & 0xFFFF;
  entry->base_mid = (base >> 16) & 0xFF;
  entry->base_hi = (base >> 24) & 0xFF;

  entry->limit_low = limit & 0xFFFF;
  entry->limit_hi = (limit >> 16) & 0xF;

  entry->flags1 = flags & 0xFF;
  entry->flags2 = (flags >> 8) & 0xF;
}

static void setup_gdt() {
  memset(&gdt_entries, 0, sizeof(gdt_entry_t) * NUM_SEGMENT);

  // null segment
  set_gdt_entry(&gdt_entries[0], 0x00000000, 0x00000000, 0b000000000000);
  // kernel code segment: ring0, r-x
  set_gdt_entry(&gdt_entries[1], 0x00000000, 0xFFFFFFFF, 0b110010011010);
  // kernel data segment: ring0, rw-
  set_gdt_entry(&gdt_entries[2], 0x00000000, 0xFFFFFFFF, 0b110010010010);
  // user code segment: ring3, r-x
  set_gdt_entry(&gdt_entries[3], 0x00000000, 0xFFFFFFFF, 0b110011111010);
  // user data segment: ring3, rw-
  set_gdt_entry(&gdt_entries[4], 0x00000000, 0xFFFFFFFF, 0b110011110010);
}

static void load_gdt() {
  gdt.base = gdt_entries;
  gdt.limit = sizeof(gdt_entry_t) * NUM_SEGMENT - 1;

  asm volatile("lgdt gdt;"
               "mov ax, 0x10;"
               "mov ds, ax;"
               "mov es, ax;"
               "mov fs, ax;"
               "mov gs, ax;"
               "mov ss, ax;"
               "jmp 0x08, .load_gdt.flush;"
               ".load_gdt.flush:");
}

void segmentation_init() {
  setup_gdt();
  load_gdt();
}
