NASM = nasm
CC = x86_64-linux-musl-gcc
BUILD = build
IMG = $(BUILD)/disk.img

BOOT_SRC = arch/x86_64/boot/stage1.asm arch/x86_64/boot/stage2.asm
BOOT_BIN = $(patsubst %.asm,$(BUILD)/%.bin,$(BOOT_SRC))

KERNEL_SRC = kernel/kernel.c $(wildcard libk/src/*.c)
KERNEL_ELF = $(BUILD)/kernel/kernel.elf
CFLAGS = -ffreestanding -nostdlib -mno-red-zone -mcmodel=kernel -fno-pic -fno-pie -no-pie -Ilibk/include

all: $(IMG)

$(BUILD)/arch/x86_64/boot/%.bin: arch/x86_64/boot/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f bin $< -o $@

$(KERNEL_ELF): $(KERNEL_SRC) linker.ld
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -T linker.ld $(KERNEL_SRC) -o $@

$(IMG): $(BOOT_BIN) $(KERNEL_ELF)
	@mkdir -p $(dir $@)
	dd if=/dev/zero of=$@ bs=512 count=4096
	dd if=$(BUILD)/arch/x86_64/boot/stage1.bin of=$@ bs=512 count=1 conv=notrunc
	dd if=$(BUILD)/arch/x86_64/boot/stage2.bin of=$@ bs=512 seek=1 conv=notrunc
	dd if=$(KERNEL_ELF) of=$@ bs=512 seek=10 conv=notrunc

run: $(IMG)
	qemu-system-x86_64 -hda $(IMG) -serial stdio -d int

gdb: $(IMG)
	qemu-system-i386 -hda $(IMG) -s -S

.PHONY: clean
clean:
	rm -rf $(BUILD)
