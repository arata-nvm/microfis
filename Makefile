BUILD=build
TARGET=kernel

.PHONY: all
all:
	mkdir -p $(BUILD)
	cd $(BUILD) && cmake .. && make
	cp $(BUILD)/$(TARGET) $(TARGET)

.PHONY: run
run: all
	qemu-system-x86_64 -kernel $(TARGET)

.PHONY: debug
debug: all
	qemu-system-i386 -kernel $(TARGET) -s -S

.PHONY: clean
clean:
	-rm -rf $(BUILD) $(TARGET)
