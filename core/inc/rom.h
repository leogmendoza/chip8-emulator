#pragma once

#include <stdint.h>

#include "chip8.h"

// Font Set
#define CHARACTER_COUNT 16
#define CHARACTER_SPRITE_SIZE 5
#define FONT_SET_SIZE (CHARACTER_COUNT * CHARACTER_SPRITE_SIZE)
#define MAX_ROM_SIZE (PROGRAM_END_ADDRESS - PROGRAM_START_ADDRESS)

void rom_init(Chip8* chip8);
void rom_load_font(Chip8* chip8);
void rom_load_rom(Chip8* chip8, const char *path);