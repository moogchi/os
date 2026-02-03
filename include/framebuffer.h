#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t *address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t bpp;
    bool initialized;
} framebuffer_t;

// Initialize framebuffer from multiboot info
void framebuffer_init(uint64_t addr, uint32_t width, uint32_t height, uint32_t pitch, uint8_t bpp);

// Drawing functions
void framebuffer_clear(uint32_t color);
void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color);
void framebuffer_fill_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void framebuffer_draw_char(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
void framebuffer_print(const char *str, uint32_t x, uint32_t y, uint32_t color);

// Color helpers
#define RGB(r, g, b) ((uint32_t)((r) << 16) | ((g) << 8) | (b))

// Common colors
#define COLOR_BLACK   RGB(0, 0, 0)
#define COLOR_WHITE   RGB(255, 255, 255)
#define COLOR_RED     RGB(255, 0, 0)
#define COLOR_GREEN   RGB(0, 255, 0)
#define COLOR_BLUE    RGB(0, 0, 255)
#define COLOR_CYAN    RGB(0, 255, 255)
#define COLOR_MAGENTA RGB(255, 0, 255)
#define COLOR_YELLOW  RGB(255, 255, 0)
#define COLOR_GRAY    RGB(128, 128, 128)

// Get framebuffer info
framebuffer_t* framebuffer_get(void);

#endif // FRAMEBUFFER_H
