#include "rom_catalog.h"

#ifdef PLATFORM_STM32

// -- ROM List --
const char *rom_list[] = {
    "tetris",
    "space_invaders",
    "pong"
};

const int rom_count = (int)(sizeof(rom_list) / sizeof(rom_list[0]));

// -- ROM Hex Byte Arrays -- 


#endif