#include "rom_catalog.h"

#ifdef PLATFORM_PC

const char *rom_list[] = {
    "../roms/tetris.ch8",
    "../roms/space_invaders.ch8",
    "../roms/pong.ch8",
    "../roms/brix.ch8",
    "../roms/connect_4.ch8",
    "../roms/puzzle.ch8",
    "../roms/blinky.ch8",
    "../test/test_opcode.ch8"
};

const int rom_count = (int)(sizeof(rom_list) / sizeof(rom_list[0]));

#endif