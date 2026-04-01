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
kernel/init.c \
kernel/kernel.c \
kernel/panic.c \
arch/x86_64/cpu/gdt.c \
arch/x86_64/cpu/tss.c \
libk/src/kprintf.c

# Kernel ASM files (interrupts)
ASM_SRC = $(wildcard arch/x86_64/interrupts/*.asm)

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
CFLAGS = -ffreestanding -nostdlib -mno-red-zone -mcmodel=kernel \
         -fno-pic -fno-pie -no-pie \
         -Iinclude -Iinclude/arch/x86_64 -Ilibk/include

# -------------------------------
# Default target
# -------------------------------
all: $(IMG)

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
gdb: $(IMG)
	qemu-system-x86_64 -hda $(IMG) -s -S

# -------------------------------
# Clean
# -------------------------------
.PHONY: clean
clean:
	rm -rf $(BUILD)
