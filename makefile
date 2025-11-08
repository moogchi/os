# Makefile for Bare Bones OSDev tutorial

# Cross-compiler prefix
CC=i686-elf-gcc
AS=i686-elf-as

# Files
KERNEL_SRC=kernel.c
BOOT_SRC=boot.s
LINKER_SCRIPT=linker.ld

# Output files
KERNEL_OBJ=kernel.o
BOOT_OBJ=boot.o
KERNEL_BIN=myos.bin
ISO_DIR=isodir
ISO_FILE=myos.iso
GRUB_CFG=grub.cfg

# Compiler flags
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

all: $(ISO_FILE)

# Assemble bootloader
$(BOOT_OBJ): $(BOOT_SRC)
	$(AS) $< -o $@

# Compile kernel
$(KERNEL_OBJ): $(KERNEL_SRC)
	$(CC) $(CFLAGS) $< -o $@

# Link kernel and bootloader
$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(LINKER_SCRIPT)
	$(CC) -T $(LINKER_SCRIPT) -o $@ $(BOOT_OBJ) $(KERNEL_OBJ) $(LDFLAGS)

# Create ISO image
$(ISO_FILE): $(KERNEL_BIN) $(GRUB_CFG)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

# Clean everything
clean:
	rm -f $(BOOT_OBJ) $(KERNEL_OBJ) $(KERNEL_BIN)
	rm -rf $(ISO_DIR)
	rm -f $(ISO_FILE)
