NASM = nasm
IMG = disk.img

ASM = stage1.asm stage2.asm
BIN = stage1.bin stage2.bin

all: $(IMG)

%.bin: %.asm
	$(NASM) -f bin $< -o $@

$(IMG): $(BIN)
	dd if=/dev/zero of=$(IMG) bs=512 count=4096
	dd if=stage1.bin of=$(IMG) bs=512 count=1 conv=notrunc
	dd if=stage2.bin of=$(IMG) bs=512 seek=1 conv=notrunc

run: $(IMG)
	qemu-system-i386 -hda $(IMG)

gdb: $(IMG)
	qemu-system-i386 -hda $(IMG) -s -S

clean:
	rm -rf *.bin *.img
