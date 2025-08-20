#include <stdio.h>

#include "rom.h"
#include "memory.h"

/* Each letter is represented by 5 rows of bytes. Take "F" for example:
 * 11110000
 * 10000000
 * 11110000
 * 10000000
 * 10000000
 */
static const uint8_t font_set[FONT_SET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0
	0x20, 0x60, 0x20, 0x20, 0x70,   // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
	0x90, 0x90, 0xF0, 0x10, 0x10,   // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
	0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
	0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
	0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
	0xF0, 0x80, 0xF0, 0x80, 0x80    // F
};

void rom_init(Chip8* chip8) {
    rom_load_font(chip8);
}

/* Copy font set into reserved section for characters */
void rom_load_font(Chip8* chip8) {
    for (size_t i = 0; i < FONT_SET_SIZE; ++i) {
        chip8->memory[FONT_SET_START_ADDRESS + i] = font_set[i];
    }
}

void rom_load_rom(Chip8* chip8, const char *path) {
	// Fill a buffer with file contents to read bytes into program memory
	uint8_t buffer[MAX_ROM_SIZE];
	int size = platform_load_rom(buffer, sizeof(buffer), path);

	if (size < 0) {
		printf("ROM source could not be loaded: %s", *path);

		return;
	}

	for (long i = 0; i < size; i++) {
		chip8->memory[PROGRAM_START_ADDRESS + i] = buffer[i];
	}

	printf("ROM source successfully loaded: %s (%d bytes)", *path, size);

	return;
}