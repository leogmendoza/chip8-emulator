#pragma once

#include <stdint.h>

/* Usable memory begins after reserved space (0x000-0x1FF) + built-in chars (0x050-0x0A0) */
#define RESERVED_START_ADDRESS 0x000
#define FONT_SET_START_ADDRESS 0x050
#define PROGRAM_START_ADDRESS 0x200

