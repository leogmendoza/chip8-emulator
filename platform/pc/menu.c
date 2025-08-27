#include "menu.h"

#ifdef PLATFORM_PC

#include "chip8.h"
#include <string.h>

static MenuState state;
static int startup_frames;
static int current_selection;

// ROM list
static const *rom_list[] = {
    "roms/tetris.ch8",
    "roms/space_invaders.ch8",
    "roms/pong.ch8"
};

static const int rom_count = sizeof(rom_list) / sizeof(rom_list[0]);

void platform_menu_init(void) {
    state = MENU_STARTUP;
    startup_frames = 120;
    current_selection = 0;

    return;
}

MenuState platform_menu_update(Chip8* chip8, const uint8_t keypad[16]) {
    switch (state) {
        case MENU_STARTUP:
            if (--startup_frames <= 0) {
                state = MENU_SELECT;
            }

            break;

        case MENU_SELECT:
            // LEFT
            if (keypad[1]) {
                current_selection = (current_selection - 1 + rom_count) % rom_count;
            }

            // RIGHT
            if (keypad[2]) {
                current_selection = (current_selection + 1) % rom_count;
            }

            // CONFIRM
            if (keypad[3]) {
                rom_load_rom(chip8, rom_list[current_selection]);
                state = MENU_IDLE;
            }

            break;

        case MENU_IDLE:
            // Handled in main loop

            break;
    }

    return state;
}

void platform_menu_draw(uint16_t *framebuffer) {
    memset(framebuffer, 0, DISPLAY_SIZE);

    if (state == MENU_STARTUP) {
        // TODO: Draw starup screen 
    } else if (state == MENU_SELECT) {
        // TODO: Draw name of current ROM selected (hovered over)
    }

    return;
}

#endif