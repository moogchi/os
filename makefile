# Makefile for Copium OS - Multi-Component Kernel

# Cross-compiler prefix
CC=i686-elf-gcc
AS=i686-elf-as

# Directories
SRC_DIR=src
BUILD_DIR=build
ISO_DIR=$(BUILD_DIR)/isodir
INCLUDE_DIR=include

# Source files
BOOT_SRC=$(SRC_DIR)/boot.s
KERNEL_SOURCES=$(wildcard $(SRC_DIR)/kernel/*.c) \
               $(wildcard $(SRC_DIR)/debug/*.c) \
               $(wildcard $(SRC_DIR)/env/*.c) \
               $(wildcard $(SRC_DIR)/mcu/*.c) \
               $(wildcard $(SRC_DIR)/events/*.c) \
               $(wildcard $(SRC_DIR)/visual/*.c)

LINKER_SCRIPT=linker.ld
GRUB_CFG=grub.cfg

# Output files
BOOT_OBJ=$(BUILD_DIR)/boot.o
KERNEL_OBJS=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(KERNEL_SOURCES))
KERNEL_BIN=$(BUILD_DIR)/myos.bin
ISO_FILE=$(BUILD_DIR)/myos.iso

# Compiler flags
CFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS=-ffreestanding -O2 -nostdlib -lgcc

all: $(ISO_FILE)

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/kernel
	mkdir -p $(BUILD_DIR)/debug
	mkdir -p $(BUILD_DIR)/env
	mkdir -p $(BUILD_DIR)/mcu
	mkdir -p $(BUILD_DIR)/events
	mkdir -p $(BUILD_DIR)/visual

# Assemble bootloader
$(BOOT_OBJ): $(BOOT_SRC) | $(BUILD_DIR)
	$(AS) $< -o $@

# Compile kernel sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Link kernel and bootloader
$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJS) $(LINKER_SCRIPT)
	$(CC) -T $(LINKER_SCRIPT) -o $@ $(BOOT_OBJ) $(KERNEL_OBJS) $(LDFLAGS)

# Create ISO image
$(ISO_FILE): $(KERNEL_BIN) $(GRUB_CFG)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

# Run in QEMU
run: $(ISO_FILE)
	qemu-system-i386 -cdrom $(ISO_FILE)

# Clean everything
clean:
	rm -rf $(BUILD_DIR)

# Docker build (same as all, but explicit)
docker-build: all

.PHONY: all clean run docker-build
