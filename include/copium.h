#ifndef COPIUM_H
#define COPIUM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Forward declaration
typedef struct multiboot_info multiboot_info_t;

// Component initialization
void kernel_init(uint32_t magic, multiboot_info_t *mbi);
void debug_init(void);
void env_init(void);
void mcu_init(void);
void events_init(void);
void visual_init(void);

#endif // COPIUM_H
