#include "chip8.h"

void chip8_init(Chip8* chip8) {
    // Initialize Program Counter at unreserved memory
    chip8->pc = PROGRAM_START_ADDRESS;

    rom_init(chip8);
}
