#include <stdio.h>

#include "chip8.h"
#include "cpu.h"
#include "rom.h"
#include "display.h"

int main(int argc, char *argv[]) {
    printf("Booting up the Chip-8 Emulator!\n");

    // No ROM path specified
    if (argc <= 1) {
        printf("Usage: %s <ROM path>\n", argv[0]);

        return 1;
    }

    Chip8 chip8;
    chip8_init(&chip8);

    rom_load_rom(&chip8, argv[1]);

    if (display_init() != 0) {
        printf("Display could not be initialized");

        return 1;
    }

    while(1) {
        cpu_cycle(&chip8);
    }
    
    return 0;
}