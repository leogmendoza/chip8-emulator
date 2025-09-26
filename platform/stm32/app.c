#include <stdio.h>

#include "stm32f1xx.h"

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
#include "rom_catalog.h"

#include <string.h>

extern int general_index;

int app(void) {
    int instructions_per_frame = DEFAULT_INSTRUCTIONS_PER_FRAME;
    int cpu_cycle_delay_ms = DEFAULT_CPU_CYCLE_DELAY_MS;

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
    }

    if (!running) {
        // User closed the window from the menu
        platform_sound_destroy();
        platform_display_destroy();
        platform_input_destroy();

        return 0;
    }

    // Apply ROM-specific overrides based on selected ROM
    const char *selected_rom = rom_list[general_index];

    for (int i = 0; i < rom_config_count; i++) {
        if (strcmp(rom_configs[i].name, selected_rom) == 0) {
            instructions_per_frame = rom_configs[i].instructions_per_frame;
            cpu_cycle_delay_ms     = rom_configs[i].cpu_cycle_delay_ms;
            
            break;
        }
    }

    if (platform_timing_init(cpu_cycle_delay_ms) != 0) {
        printf("Timing could not be initialized");

        return 1;
    }

    while(running) {
        // Handle window closing gracefully
        running = platform_display_poll_events();

        // Poll for pressed inputs
        platform_input_update(chip8.keypad);

        // HACK: Press F key instead of RST since the big, beautiful red button was not tied high to 3V3 (only NRST)
        if (chip8.keypad[0xF]) {
            NVIC_SystemReset(); 
        }

        // Run multiple instructions per frame
        for (size_t i = 0; i < instructions_per_frame; ++i) {
            cpu_cycle(&chip8);
        }

        // Draw once per frame
        platform_display_draw(chip8.display);

        // Beep while sound timer is activated
        platform_sound_update(chip8.sound_timer > 0);

        // Ensure ticking at desired FPS
        platform_timing_sync(&chip8);
    }
    
    platform_sound_destroy();
    platform_display_destroy();
    platform_input_destroy();
    platform_timing_destroy();

    return 0;
}
