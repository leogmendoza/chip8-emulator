#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "rom_loader.h"

#ifdef PLATFORM_PC

uint8_t platform_rom_loader_read_mem(Chip8* chip8, uint16_t address) {
    return chip8->memory[address];
}

void platform_rom_loader_write_mem(Chip8* chip8, uint16_t address, uint8_t value) {
    chip8->memory[address] = value;
}

int platform_rom_loader_load_rom(uint8_t *buffer, size_t max_size, const char *path) {
    // Open file in binary mode
	FILE *file = fopen(path, "rb");
	
	if (file == NULL) {
		return -1;
	}

    // Get file size by jumping to the end position
	fseek(file, 0, SEEK_END);
	long size = ftell(file);


    if (size > max_size) {
        fclose(file);
        
        return -1;
    }

    rewind(file);
	fread(buffer, 1, size, file);
	fclose(file);

	return size;
}

#endif