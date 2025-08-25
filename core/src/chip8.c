#include <string.h>

#include "chip8.h"
#include "rom.h"
#include "cpu.h"
#include "timers.h"

void chip8_init(Chip8* chip8) {
    // Clear everything
    memset(chip8->V, 0, sizeof(chip8->V));
    memset(chip8->memory, 0, sizeof(chip8->memory));
    memset(chip8->display, 0, sizeof(chip8->display));

    chip8->I = 0;
    chip8->SP = 0;   

    // Initialize Program Counter at unreserved memory
    chip8->PC = PROGRAM_START_ADDRESS;

    rom_init(chip8);
    cpu_init(chip8);
    timers_init(chip8);
}   
