# === Tools ===
AS      = nasm
CC      = x86_64-elf-gcc
LD      = x86_64-elf-ld
CFLAGS  = -ffreestanding -m64 -O2 -Wall -Wextra -Iinclude
LDFLAGS = -T linker.ld -nostdlib

# === Directories and files ===
BUILD_DIR = build
KERNEL_SRC := $(shell find kernel libc gui userland -name "*.c")
KERNEL_OBJ := $(patsubst %, $(BUILD_DIR)/%.o, $(basename $(KERNEL_SRC)))

STAGE1_SRC = bootloader/stage1.asm
STAGE2_SRC = bootloader/stage2.asm
STAGE1_BIN = $(BUILD_DIR)/stage1.bin
STAGE2_BIN = $(BUILD_DIR)/stage2.bin

KERNEL_ELF = $(BUILD_DIR)/kernel.elf
OS_IMAGE   = S-havlOS.img

# === Main rule ===
all: $(OS_IMAGE)

# === Create final image (stage1 + stage2 + kernel) ===
$(OS_IMAGE): $(STAGE1_BIN) $(STAGE2_BIN) $(KERNEL_ELF)
	cat $(STAGE1_BIN) $(STAGE2_BIN) $(KERNEL_ELF) > $@

# === Compile C to object ===
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# === Link the kernel ===
$(KERNEL_ELF): $(KERNEL_OBJ) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJ)

# === Assemble the bootloader ===
$(STAGE1_BIN): $(STAGE1_SRC)
	@mkdir -p $(dir $@)
	$(AS) -f bin $< -o $@

$(STAGE2_BIN): $(STAGE2_SRC)
	@mkdir -p $(dir $@)
	$(AS) -f bin $< -o $@

# === Clean generated files ===
clean:
	rm -rf $(BUILD_DIR) $(OS_IMAGE)

.PHONY: all clean

