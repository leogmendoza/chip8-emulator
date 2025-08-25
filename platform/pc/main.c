#include <stdio.h>

#include "chip8.h"
#include "cpu.h"
#include "rom.h"
#include "display.h"
#include "input.h"
#include "timers.h"
#include "timing.h"

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

    if (platform_display_init() != 0) {
        printf("Display could not be initialized");

        return 1;
    }

    if (platform_input_init() != 0) {
        printf("Input could not be initialized");

        return 1;
    }

    if (platform_timing_init(CPU_CYCLE_DELAY_MS) != 0) {
        printf("Timing could not be initialized");

        return 1;
    }

    int running = 1;

    while(running) {
        // Handle window closing gracefully
        running = platform_display_poll_events();

        // Poll for pressed inputs
        platform_input_update(chip8.keypad);

        // Run multiple instructions per frame
        for (size_t i = 0; i < INSTRUCTIONS_PER_FRAME; ++i) {
            cpu_cycle(&chip8);
        }

        // Draw once per frame
        platform_display_draw(chip8.display);

        // Ensure ticking at desired FPS
        platform_timing_sync(&chip8);
    }
    
    platform_display_destroy();
    platform_input_destroy();
    platform_timing_destroy();

    return 0;
}