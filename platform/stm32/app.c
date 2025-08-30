#include <stdio.h>

#include "app.h"
#include "chip8.h"
#include "platform.h"
#include "cpu.h"
#include "rom.h"
#include "display.h"
#include "input.h"
#include "timers.h"
#include "timing.h"
#include "sound.h"
#include "menu.h"

int app(void) {
    Chip8 chip8;
    chip8_init(&chip8);

    // Set up platform subsystems
    if (platform_display_init(DISPLAY_SCALE) != 0) {
        printf("Display could not be initialized");

        return 1;
    }

    if (platform_input_init() != 0) {
        printf("Input could not be initialized");

        return 1;
    }

    // if (platform_timing_init(CPU_CYCLE_DELAY_MS) != 0) {
    //     printf("Timing could not be initialized");

    //     return 1;
    // }

    if (platform_sound_init() != 0) {
        printf("Sound could not be initialized");

        return 1;
    }

    int running = 1;


    // Pre-ROM loop
    platform_menu_init();

    while (running) {
        // Handle window closing gracefully during startup
        running = platform_display_poll_events();

        if (!running) {
            break;
        }

        // Update keypad and feed to the menu
        platform_input_update(chip8.keypad);

        if (platform_menu_update(&chip8, chip8.keypad) == MENU_IDLE) {
            break;
        }


        // Draw the menu and keep audio silent
        platform_menu_draw();

        // Maintain frame pacing (same clock as the emulator)
        // TODO: UNCOMMENT -> platform_timing_sync(&chip8);
    }

    if (!running) {
        // // User closed the window from the menu
        platform_sound_destroy();
        platform_display_destroy();
        platform_input_destroy();
        // platform_timing_destroy();

        return 0;
    }

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

        // Beep while sound timer is activated
        platform_sound_update(chip8.sound_timer > 0);

        // // Ensure ticking at desired FPS
        // platform_timing_sync(&chip8);
    }
    
    platform_sound_destroy();
    platform_display_destroy();
    platform_input_destroy();
    // platform_timing_destroy();

    return 0;
}