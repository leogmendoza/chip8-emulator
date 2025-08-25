#pragma once

#include <stdint.h>

#include "chip8.h"

#define TIMER_CLOCK_FREQUENCY_HZ 60

void timers_init(Chip8 *chip8);
void timers_tick(uint8_t *delay_timer, uint8_t *sound_timer);