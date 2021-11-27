TARGET=kernel
OBJ=common.o console.o interrupt.o io.o isr.o kernel.o segmentation.o start.o

SRC_DIR=src
INCLUDE_DIR=include

CCFLAGS=-g -m32 -masm=intel -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -Werror
LDFLAGS=-Tlink.ld -melf_i386
ASFLAGS=--32 -msyntax=intel

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: $(SRC_DIR)/%.c
	$(CC) $(CCFLAGS) -I $(INCLUDE_DIR) -c -o $@ $<

%.o: $(SRC_DIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

.PHONY: run
run: all
	qemu-system-x86_64 -kernel $(TARGET)

.PHONY: clean
clean:
	-rm -rf $(OBJ) $(TARGET)