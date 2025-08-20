#include <stdio.h>

#include "platform.h"

int platform_load_rom(uint8_t *buffer, size_t max_size, const char *path) {
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