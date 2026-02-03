# Development Log: Copium OS

**Project:** Custom x86 Operating System with Graphics Support  
**Architecture:** i386 (32-bit x86)  
**Bootloader:** GRUB2 with Multiboot v2 specification  
**Development Environment:** Docker (Ubuntu 24.04), i686-elf cross-compiler  
**Objective:** Bootable OS with framebuffer graphics and modular component architecture

---

## Technical Challenges and Solutions

### 1. Multiboot Version Mismatch (January 29, 2026)

**Problem:** System had inconsistent multiboot implementations across different files causing potential boot failures.

**Root Cause:**

- boot.s was using Multiboot v1 header and magic number (0x1BADB002)
- multiboot.h declared Multiboot v2 structures and magic (0x36D76289)
- main.c referenced undefined MULTIBOOT_BOOTLOADER_MAGIC constant
- grub.cfg used v1 'multiboot' command
- Mixing protocols would cause bootloader to fail reading header correctly

**Solution:**

1. Converted boot.s to use proper Multiboot v2 tag-based header format
   - Changed magic from 0x1BADB002 to 0xe85250d6 (MB2_MAGIC)
   - Replaced flat header with tag-based structure (framebuffer tag + end tag)
   - Added proper alignment and checksum calculation for v2
2. Fixed main.c magic check to use MULTIBOOT2_BOOTLOADER_MAGIC
3. Updated grub.cfg to use 'multiboot2' command instead of 'multiboot'

**Result:**

- Eliminated version conflict - entire system now uses Multiboot v2 consistently
- Kernel can now properly parse Multiboot v2 information structures
- GRUB correctly identifies and loads kernel using v2 protocol
- Magic number validation works correctly (0x36d76289)
- Access to better v2 features like tag-based information passing
- Framebuffer request now uses proper v2 tag format for reliable graphics mode setup

---

### 2. Black Screen on Boot - Missing Framebuffer Initialization (February 3, 2026)

**Problem:** System booted to completely black screen after GRUB selection despite requesting framebuffer.

**Root Cause:**

- Multiboot v2 was requesting 1024x768x32bpp framebuffer in boot.s
- Framebuffer code existed in framebuffer.c but was never initialized
- kernel_init() was using VGA text mode (0xB8000) instead of framebuffer
- No parsing of Multiboot v2 tags to extract framebuffer information
- Framebuffer was allocated but never written to, resulting in black screen

**Diagnostic Process:**

- Verified Multiboot v2 header was correctly formatted
- Confirmed GRUB was passing framebuffer information via tags
- Identified missing tag parsing logic in kernel initialization
- Recognized VGA text mode (0xB8000) doesn't work in graphics mode

**Solution:**

1. Added parse_multiboot2_tags() function to iterate through Multiboot v2 tags
2. Implemented framebuffer tag detection (MULTIBOOT_TAG_TYPE_FRAMEBUFFER)
3. Called framebuffer_init() with extracted address, width, height, pitch, and bpp
4. Replaced terminal_initialize() with framebuffer_clear() to set blue background
5. Switched from terminal_writestring() to framebuffer_print() for RGB text output
6. Added VGA text mode as fallback if framebuffer not available

**Result:**

- Screen now displays blue background instead of black (framebuffer is active)
- Multiboot v2 tag parsing correctly extracts 1024x768x32bpp framebuffer info
- System now uses full 16.7 million color graphics mode instead of 16-color text mode
- Proper initialization prevents black screen by actually writing to framebuffer memory
- Fallback ensures system boots even without framebuffer support

---

### 3. Broken/Garbage Text Rendering in Framebuffer (February 3, 2026)

**Problem:** Text appeared as broken/corrupted characters on blue screen. Only "COPIUM. !" characters rendered correctly.

**Root Cause:**

- Incomplete 8x8 bitmap font definition in framebuffer.c
- Font array only contained 9 character definitions out of 128 possible ASCII characters
- Characters needed for boot messages (v, 0, 1, lowercase letters) were undefined
- Undefined characters had empty glyph data (all 0x00), causing blank/garbage rendering
- framebuffer_draw_char() was accessing undefined array elements

**Solution:**

1. Expanded font_8x8 array from 9 characters to 45+ characters
2. Added all uppercase letters A-Z with proper 8x8 bitmap patterns
3. Added all lowercase letters a-z with proper 8x8 bitmap patterns
4. Added numbers 0-3 for version string support
5. Added punctuation: space, period, comma, exclamation mark
6. Used proper 8x8 bitmap encoding for each character

**Result:**

- Text now renders correctly with readable characters
- "Copium OS v0.1.0" displays properly with working version numbers
- Status messages appear as intended instead of garbage
- System boot messages are now human-readable
- Font covers most common ASCII characters needed for boot messages
- Future text output will work for most standard English text

---

## System Architecture

### Current Configuration

**Boot Process:**

- GRUB2 bootloader with Multiboot v2 specification
- Custom boot.s assembly initializes stack and calls kernel_main
- Multiboot v2 tag parsing extracts hardware information
- Framebuffer initialization or VGA text mode fallback

**Display System:**

- Primary: 1024x768x32bpp linear framebuffer (16.7M colors)
- Fallback: VGA text mode 80x25 (16 colors)
- Custom 8x8 bitmap font renderer
- RGB color support with COLOR\_\* macros

**Component Architecture:**

- debug: Debug utilities and diagnostics
- env: Environment management
- mcu: Microcontroller interface stubs
- events: Event management system
- visual: Framebuffer graphics driver
- kernel: Main kernel initialization and control

---

## Build Environment

**Cross-Compiler:** i686-elf-gcc 7.1.0  
**Assembler:** i686-elf-as  
**ISO Builder:** grub-mkrescue with xorriso  
**Emulator:** QEMU i386  
**Container:** Docker with Ubuntu 24.04 base

**Docker Workflow:**

```bash
# Build OS
docker compose run --rm build

# Interactive development
docker compose run --rm dev

# Run in QEMU
make run
```

---

## Future Optimization Opportunities

### Display System

- Add complete ASCII font (128 characters)
- Implement scrolling for text output
- Add line buffering and newline handling
- Consider using PSF (PC Screen Font) format
- Implement cursor rendering

### Boot Process

- Parse additional Multiboot v2 tags (memory map, boot loader name)
- Implement proper memory management
- Add error handling for missing required tags

### Component Architecture

- Implement actual functionality in component stubs
- Add inter-component communication
- Develop proper subsystem initialization order

---

## Current Status

**Working:**

- Multiboot v2 compliant boot process
- Framebuffer graphics initialization (1024x768x32bpp)
- Basic text rendering with RGB colors
- VGA text mode fallback
- Serial port debug output (COM1)
- Component initialization framework

**In Progress:**

- Complete font character set
- Text scrolling and console features
- Component functionality implementation

**Planned:**

- Keyboard input handling
- Memory management
- Process scheduling
- File system support

---

## Development Lessons

1. **Bootloader Protocol Consistency:** Mixing Multiboot versions causes silent failures. Always verify header, magic numbers, and bootloader commands match.

2. **Framebuffer Initialization:** Requesting a framebuffer isn't enough - you must parse Multiboot tags, extract the address, and actually write to it.

3. **Font Completeness:** Bitmap fonts require explicit definition for every character. Undefined characters render as garbage or blank.

4. **Cross-Compilation:** OS development requires cross-compiler toolchain. Native compiler produces code for existing OS, not bare metal.

5. **Docker for Reproducibility:** Containerized build environment ensures consistent toolchain across development machines.

---

**Last Updated:** February 3, 2026  
**Status:** Active Development - Basic framebuffer graphics working
