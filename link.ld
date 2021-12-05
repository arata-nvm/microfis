OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
  . = 0x100000;

  __kernel_start = .;

  .text ALIGN(0x1000):
  {
    *(.multiboot)
    *(.text)
  }

  .rodata ALIGN(0x1000):
  {
    *(.rodata)
  }

  .data ALIGN(0x1000):
  {
    *(.data)
  }

  .bss ALIGN(0x1000):
  {
    *(.bss)
  }

  __kernel_end = .;
}