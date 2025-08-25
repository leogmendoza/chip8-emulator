#pragma once

#include "platform.h"
#include "chip8.h"

// How many ms between CPU cycles (tune per ROM)
#define CPU_CYCLE_DELAY_MS 2

int platform_timing_init(int cycle_delay_ms);
void platform_timing_destroy(void);
void platform_timing_sync(Chip8* chip8);