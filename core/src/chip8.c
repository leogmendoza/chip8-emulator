#include "chip8.h"

void chip8_init(Chip8* chip8) {
    // Clear registers and stack
    memset(chip8->V, 0, sizeof(chip8->V));
    chip8->I = 0;
    chip8->SP = 0;

    // Reset timers
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;     

    // Initialize Program Counter at unreserved memory
    chip8->PC = PROGRAM_START_ADDRESS;

    rom_init(chip8);
}   
