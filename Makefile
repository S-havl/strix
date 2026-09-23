# -------------------------------
# Toolchain
# -------------------------------
NASM = nasm
CC   = x86_64-elf-gcc

# -------------------------------
# Build folders
# -------------------------------
BUILD = build
IMG   = $(BUILD)/disk.img

# -------------------------------
# Source files
# -------------------------------
# Bootloader
BOOT_SRC = arch/x86_64/boot/stage1.asm arch/x86_64/boot/stage2.asm
BOOT_BIN = $(patsubst %.asm,$(BUILD)/%.bin,$(BOOT_SRC))

# Kernel C files
SRC = \
kernel/core/init.c \
kernel/core/kernel.c \
kernel/core/panic.c \
kernel/core/interrupt.c \
arch/x86_64/cpu/gdt.c \
arch/x86_64/cpu/gdt_flush.c \
arch/x86_64/cpu/tss.c \
arch/x86_64/cpu/tss_flush.c \
arch/x86_64/interrupts/idt.c \
drivers/video/vga/vga.c \
libk/src/kprintf.c \
arch/x86_64/interrupts/pic.c \
mm/pmm.c

# Kernel ASM files (interrupts)
ASM_SRC = $(wildcard arch/x86_64/interrupts/*.asm)

FORMAT_FILES := $(shell find . -type d -name '$(BUILD)' -prune -false -o -type f \( -name '*.c' -o -name '*.h' \))

# -------------------------------
# Object files
# -------------------------------
OBJ     = $(patsubst %.c,$(BUILD)/%.o,$(SRC))
ASM_OBJ = $(patsubst %.asm,$(BUILD)/%.o,$(ASM_SRC))

# Final kernel ELF
KERNEL_ELF = $(BUILD)/kernel/kernel.elf

# -------------------------------
# Compiler flags
# -------------------------------
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=gnu11 -Wno-unused-parameter -Wno-unused-function -g -ffreestanding -nostdlib -mno-red-zone -mcmodel=kernel \
         -fno-pic -fno-pie -no-pie \
         -Iinclude -Iinclude/arch/x86_64 -Ilibk/include

.PHONY: all format run gdb-64 gdb-32 gdb-16 clean

# -------------------------------
# Default target
# -------------------------------
all: $(IMG)

# -------------------------------
# Formatter rule
# -------------------------------
format:
	@echo "Formatting the Strix source code..."
	@clang-format -i $(FORMAT_FILES)

# -------------------------------
# Bootloader rules
# -------------------------------
$(BUILD)/arch/x86_64/boot/%.bin: arch/x86_64/boot/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f bin $< -o $@

# -------------------------------
# Kernel C rule
# -------------------------------
$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# -------------------------------
# Kernel ASM rule (interrupts)
# -------------------------------
$(BUILD)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

# -------------------------------
# Link kernel ELF
# -------------------------------
$(KERNEL_ELF): $(OBJ) $(ASM_OBJ) linker.ld
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -T linker.ld $(OBJ) $(ASM_OBJ) -o $@

# -------------------------------
# Build disk image
# -------------------------------
$(IMG): $(BOOT_BIN) $(KERNEL_ELF)
	@mkdir -p $(dir $@)
	dd if=/dev/zero of=$@ bs=512 count=4096
	dd if=$(BUILD)/arch/x86_64/boot/stage1.bin of=$@ bs=512 count=1 conv=notrunc
	dd if=$(BUILD)/arch/x86_64/boot/stage2.bin of=$@ bs=512 seek=1 conv=notrunc
	dd if=$(KERNEL_ELF) of=$@ bs=512 seek=10 conv=notrunc

# -------------------------------
# Run in QEMU
# -------------------------------
run: $(IMG)
	qemu-system-x86_64 -hda $(IMG) -serial stdio -d int

# -------------------------------
# Run in QEMU for GDB
# -------------------------------
gdb-64: $(IMG)
	qemu-system-x86_64 -hda $(IMG) -s -S

gdb-32:
	qemu-system-i386 -hda $(IMG) -s -S

gdb-16:
	qemu-system-i386 -hda $(IMG) -s -S

# -------------------------------
# Clean
# -------------------------------

clean:
	rm -rf $(BUILD)
