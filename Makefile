NASM = nasm
CC = x86_64-linux-musl-gcc
BUILD = build
IMG = $(BUILD)/disk.img

BOOT_SRC = bootloader/stage1.asm bootloader/stage2.asm
BOOT_BIN = $(patsubst %.asm,$(BUILD)/%.bin,$(BOOT_SRC))

KERNEL_SRC = kernel/x86_64/kernel.c
KERNEL_ELF = $(BUILD)/kernel/kernel.elf
CFLAGS = -ffreestanding -nostdlib -mno-red-zone -mcmodel=kernel -fno-pic -fno-pie -no-pie

all: $(IMG)

$(BUILD)/bootloader/%.bin: bootloader/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f bin $< -o $@

$(KERNEL_ELF): $(KERNEL_SRC) linker.ld
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -T linker.ld $< -o $@

$(IMG): $(BOOT_BIN) $(KERNEL_ELF)
	@mkdir -p $(dir $@)
	dd if=/dev/zero of=$@ bs=512 count=4096
	dd if=$(BUILD)/bootloader/stage1.bin of=$@ bs=512 count=1 conv=notrunc
	dd if=$(BUILD)/bootloader/stage2.bin of=$@ bs=512 seek=1 conv=notrunc
	dd if=$(KERNEL_ELF) of=$@ bs=512 seek=10 conv=notrunc

run: $(IMG)
	qemu-system-x86_64 -hda $(IMG) -serial stdio -d int

gdb: $(IMG)
	qemu-system-i386 -hda $(IMG) -s -S

.PHONY: clean
clean:
	rm -rf $(BUILD)
