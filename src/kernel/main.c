#include "../../include/copium.h"
#include "../../include/framebuffer.h"
#include "../../include/multiboot.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// check if its targeting the right os
#if defined(__linux__)
#error "You are not using the cross-compiler, stop now!"
#endif

#if !defined(__i386__)
#error "You need a i386 compiler"
#endif

enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}
/*
the function combines the foreground and background color for vga
uint8_t(1byte)
0000|0000
 bg | fg

*/

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint8_t)color << 8;
}

/*
the function combines the character and color for vga
uint16_t(2byte)
 0000,0000|0000,0000
ASCII Char|  color
*/

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}
/*
"Why can't we use standard strlen?"

For regular C code, the code assumes that it runs on an OS so we can use
standard fuctions like printf, strlen, and malloc.

Because we are writing an OS we have to write our own.
*/

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer = (uint16_t *)VGA_MEMORY;

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLUE);
  // blue background yellow font yaaaaaaaaaaaaaaaaa

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
      terminal_row = 0;
    }
  }
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    terminal_putchar(data[i]);
  }
}

void terminal_writestring(const char *data) {
  terminal_write(data, strlen(data));
}

// Serial port output for debugging
void serial_putchar(char c) {
  // COM1 port 0x3F8
  while ((*(volatile uint8_t *)0x3FD & 0x20) == 0)
    ;
  *(volatile uint8_t *)0x3F8 = c;
}

void serial_write(const char *str) {
  while (*str) {
    serial_putchar(*str++);
  }
}

// Parse Multiboot2 tags to find framebuffer info
bool parse_multiboot2_tags(struct multiboot_info *mbi) {
  struct multiboot_tag *tag;

  // Tags start after the fixed-size header
  for (tag = (struct multiboot_tag *)(mbi->tags);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag =
           (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7))) {

    if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
      struct multiboot_tag_framebuffer *fb_tag =
          (struct multiboot_tag_framebuffer *)tag;

      serial_write("Found framebuffer tag!\n");

      // Initialize framebuffer
      framebuffer_init(fb_tag->framebuffer_addr, fb_tag->framebuffer_width,
                       fb_tag->framebuffer_height, fb_tag->framebuffer_pitch,
                       fb_tag->framebuffer_bpp);

      return true;
    }
  }

  return false;
}

void kernel_init(uint32_t magic, multiboot_info_t *mbi) {
  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    serial_write("ERROR: Invalid multiboot magic\n");
    for (;;) {
      __asm__ volatile("hlt");
    }
  }

  serial_write("Copium OS - Serial Output Active\n");

  // Parse Multiboot2 tags to get framebuffer
  bool has_framebuffer = parse_multiboot2_tags(mbi);

  if (has_framebuffer) {
    serial_write("Framebuffer initialized!\n");

    // Clear screen to blue background
    framebuffer_clear(RGB(0, 0, 128));

    // Draw title
    framebuffer_print("Copium OS v0.1.0", 10, 10, COLOR_YELLOW);
    framebuffer_print("Kernel booted successfully!", 10, 20, COLOR_WHITE);
  } else {
    // Fallback to VGA text mode
    serial_write("No framebuffer, using VGA text mode\n");
    terminal_initialize();
    terminal_writestring("Copium OS v0.1.0\n");
    terminal_writestring("Kernel booted successfully!\n");
  }

  // Initialize all subsystems
  debug_init();
  env_init();
  mcu_init();
  events_init();
  visual_init();

  if (has_framebuffer) {
    framebuffer_print("All components initialized!", 10, 30, COLOR_GREEN);
    framebuffer_print("System ready.", 10, 40, COLOR_WHITE);
  } else {
    terminal_writestring("\nAll components initialized!\n");
    terminal_writestring("System ready.\n");
  }

  serial_write("All components initialized\n");
}

void kernel_main(uint32_t magic, multiboot_info_t *mbi) {
  kernel_init(magic, mbi);
}