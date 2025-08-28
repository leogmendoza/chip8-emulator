#include "rom_catalog.h"

#ifdef PLATFORM_PC

const char *rom_list[] = {
    "../roms/tetris.ch8",
    "../roms/space_invaders.ch8",
    "../roms/pong.ch8"
};

const int rom_count = (int)(sizeof(rom_list) / sizeof(rom_list[0]));

#endif