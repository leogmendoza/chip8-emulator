#pragma once

// Config params
#define DISPLAY_SCALE 20
// Overriden at runtime (right now, 8 works best for PC implementation on Tetris and Space Invaders)
#define DEFAULT_INSTRUCTIONS_PER_FRAME 10
#define DEFAULT_CPU_CYCLE_DELAY_MS 3  // Try to keep at 0 for STM32 since MCU won't benefit from delays

/* Uncomment depending on the platform */
#define PLATFORM_PC
// #define PLATFORM_STM32