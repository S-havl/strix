NASM = nasm
BUILD = build
IMG = $(BUILD)/disk.img

BOOT_SRC = bootloader/stage1.asm bootloader/stage2.asm
BOOT_BIN = $(patsubst %.asm,$(BUILD)/%.bin,$(BOOT_SRC))

all: $(IMG)

$(BUILD)/bootloader/%.bin: bootloader/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f bin $< -o $@

$(IMG): $(BOOT_BIN) kernel.elf
	@mkdir -p $(dir $@)
	dd if=/dev/zero of=$@ bs=512 count=4096
	dd if=$(BUILD)/bootloader/stage1.bin of=$@ bs=512 count=1 conv=notrunc
	dd if=$(BUILD)/bootloader/stage2.bin of=$@ bs=512 seek=1 conv=notrunc
	dd if=kernel.elf of=$@ bs=512 seek=10 conv=notrunc

run: $(IMG)
	qemu-system-x86_64 -hda $(IMG) -serial stdio -d int

gdb: $(IMG)
	qemu-system-i386 -hda $(IMG) -s -S

.PHONY: clean
clean:
	rm -rf $(BUILD)
