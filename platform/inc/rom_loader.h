#pragma once

#include "platform.h"

#include <stddef.h>
#include "chip8.h"

/* Read memory */
uint8_t platform_rom_loader_read_mem(Chip8* chip8, uint16_t address);

/* Write memory */
void platform_rom_loader_write_mem(Chip8* chip8, uint16_t address, uint8_t value);

/* Fill buffer with bytes from ROM source */
int platform_rom_loader_load_rom(uint8_t *buffer, size_t max_size, const char *path);