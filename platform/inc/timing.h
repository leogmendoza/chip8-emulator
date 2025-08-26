#pragma once

#include "platform.h"
#include "chip8.h"

int platform_timing_init(int cycle_delay_ms);
void platform_timing_destroy(void);
void platform_timing_sync(Chip8* chip8);