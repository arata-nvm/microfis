.equ MBOOT_MAGIC, 0x1BADB002
.equ MBOOT_FLAGS_PAGE_ALIGN, 0x1
.equ MBOOT_FLAGS_MEMORY_INFO, 0x2
.equ MBOOT_FLAGS, (MBOOT_FLAGS_PAGE_ALIGN | MBOOT_FLAGS_MEMORY_INFO)
.equ MBOOT_CHECKSUM, -(MBOOT_MAGIC + MBOOT_FLAGS)

.section .multiboot

mboot_header:
.long MBOOT_MAGIC
.long MBOOT_FLAGS
.long MBOOT_CHECKSUM


.section .text

.global start
start:
  push %eax
  push %ebx
  call kmain
.loop:
  jmp .loop
