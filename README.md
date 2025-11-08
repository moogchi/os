# MyOS - Bare Bones Operating System

A minimal x86 operating system following the OSDev bare bones tutorial.

## Features

- Multiboot-compliant bootloader
- Basic kernel setup
- GRUB2 bootloader integration
- Runs on x86 (i686) architecture

## Prerequisites

To build this OS, you need:

- **i686-elf cross-compiler** (GCC and binutils)
- **GRUB** (`grub-mkrescue` and `grub-common`)
- **xorriso** (for ISO creation)
- **QEMU** (for testing - `qemu-system-i386`)

### Installing the Cross-Compiler

Follow the [OSDev GCC Cross-Compiler](https://wiki.osdev.org/GCC_Cross-Compiler) guide to build your cross-compiler.

The cross-compiler should be available in your PATH or you can modify the Makefile to point to its location.

## Building

```bash
# Build everything
make all

# Clean build artifacts
make clean
```

## Running

To run the OS in QEMU:

```bash
qemu-system-i386 -cdrom myos.iso
```

## Project Structure

```
.
├── boot.s          # Assembly bootloader with multiboot header
├── kernel.c        # Main kernel code
├── linker.ld       # Linker script
├── grub.cfg        # GRUB configuration
├── makefile        # Build automation
└── README.md       # This file
```

## How It Works

1. **Bootloader** (`boot.s`): Sets up the multiboot header, stack, and calls the kernel
2. **Kernel** (`kernel.c`): The main kernel entry point
3. **Linker Script** (`linker.ld`): Organizes the memory layout
4. **GRUB**: Loads the kernel using the multiboot specification

## Resources

- [OSDev Wiki - Bare Bones](https://wiki.osdev.org/Bare_Bones)
- [OSDev Wiki - Multiboot](https://wiki.osdev.org/Multiboot)
- [GRUB Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)

## License

This is a learning project. Feel free to use and modify as needed.
