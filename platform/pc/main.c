#include <stdio.h>

#include "chip8.h"
#include "cpu.h"
#include "rom.c"

int main(int argc, char *argv[]) {
    printf("Booting up the Chip-8 Emulator!");

    // No ROM path specified
    if (argc <= 1) {
        printf("Usage: %s <ROM path>\n", argv[0]);

        return 1;
    }

    Chip8 chip8;
    chip8_init(&chip8);

    while(1) {
        cpu_cycle(&chip8);
    }
    
    return 0;
}