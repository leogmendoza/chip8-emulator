#pragma once

#include <stdint.h>

#include "chip8.h"
#include "memory.h"

// Font Set
#define CHARACTER_COUNT 16
#define CHARACTER_SPRITE_SIZE 5
#define FONT_SET_SIZE (CHARACTER_COUNT * CHARACTER_SPRITE_SIZE)

void rom_load_font(Chip8* chip8);