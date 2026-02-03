#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <stdint.h>

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36D76289

#define MULTIBOOT_TAG_TYPE_END 0
#define MULTIBOOT_TAG_TYPE_CMDLINE 1
#define MULTIBOOT_TAG_TYPE_BOOT_LOADER 2
#define MULTIBOOT_TAG_TYPE_MMAP 6
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8

/* Multiboot information structure */
struct multiboot_tag {
  uint32_t type;
  uint32_t size;
};

struct multiboot_info {
  uint32_t total_size;
  uint32_t reserved;
  struct multiboot_tag tags[];
};

/* Framebuffer tag */
struct multiboot_tag_framebuffer {
  uint32_t type;
  uint32_t size;

  uint64_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
  uint8_t framebuffer_type;
  uint16_t reserved;

  uint8_t color_info[];
};

#endif // MULTIBOOT2_H
