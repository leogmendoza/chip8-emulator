#pragma once

// Config params
#define DISPLAY_SCALE 10
// Overriden at runtime (right now, these work best for PC implementation on Tetris and Space Invaders)
#define DEFAULT_INSTRUCTIONS_PER_FRAME 50
#define DEFAULT_CPU_CYCLE_DELAY_MS 0  // Try to keep at 0 since MCU won't benefit from delays

/* Uncomment depending on the platform */
// #define PLATFORM_PC
#define PLATFORM_STM32