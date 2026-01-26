#ifndef COPIUM_H
#define COPIUM_H

#include <stddef.h>
#include <stdint.h>

// Forward declarations for all components
void kernel_init(void);
void debug_init(void);
void env_init(void);
void mcu_init(void);
void events_init(void);
void visual_init(void);

#endif // COPIUM_H
