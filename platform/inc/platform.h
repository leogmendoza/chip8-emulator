// platform.h
#pragma once

#include <stddef.h>
#include <stdint.h>

/* Fill buffer with bytes from ROM source */
int platform_load_rom(uint8_t *buffer, size_t max_size, const char *path);