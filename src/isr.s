.intel_syntax noprefix
.extern interrupt_dispatcher

.macro isr_no_err num
.global isr_\num
isr_\num:
  cli
  push 0
  push \num
  jmp isr_common
.endm

.macro isr_has_err num
.global isr_\num
isr_\num:
  cli
  push \num
  jmp isr_common
.endm

.global isr_common
isr_common:
  pusha

  # save segment descriptor
  mov ax, ds
  push eax

  # load kernel data segment
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  call interrupt_dispatcher

  # restore segment descriptor
  pop eax
  mov dx, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popa
  add esp, 0x8
  iret

isr_no_err 0
isr_no_err 1
isr_no_err 2
isr_no_err 3
isr_no_err 4
isr_no_err 5
isr_no_err 6
isr_no_err 7
isr_has_err 8
isr_no_err 9
isr_has_err 10
isr_has_err 11
isr_has_err 12
isr_has_err 13
isr_has_err 14
isr_no_err 15
isr_no_err 16
isr_no_err 17
isr_no_err 18
isr_no_err 19
isr_no_err 20
isr_no_err 21
isr_no_err 22
isr_no_err 23
isr_no_err 24
isr_no_err 25
isr_no_err 26
isr_no_err 27
isr_no_err 28
isr_no_err 29
isr_no_err 30
isr_no_err 31
isr_no_err 32
isr_no_err 33
isr_no_err 34
isr_no_err 35
isr_no_err 36
isr_no_err 37
isr_no_err 38
isr_no_err 39
isr_no_err 40
isr_no_err 41
isr_no_err 42
isr_no_err 43
isr_no_err 44
isr_no_err 45
isr_no_err 46
isr_no_err 47
