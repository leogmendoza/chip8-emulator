#include "input.h"

#ifdef PLATFORM_PC

#include <SDL2/SDL.h>

#include "chip8.h"

// Map SDL keycodes to CHIP-8 hex keypad (0x0–0xF)
static const SDL_Scancode keymap[16] = {
    SDL_SCANCODE_X,  // 0
    SDL_SCANCODE_1,  // 1
    SDL_SCANCODE_2,  // 2
    SDL_SCANCODE_3,  // 3
    SDL_SCANCODE_Q,  // 4
    SDL_SCANCODE_W,  // 5
    SDL_SCANCODE_E,  // 6
    SDL_SCANCODE_A,  // 7
    SDL_SCANCODE_S,  // 8
    SDL_SCANCODE_D,  // 9
    SDL_SCANCODE_Z,  // A
    SDL_SCANCODE_C,  // B
    SDL_SCANCODE_4,  // C
    SDL_SCANCODE_R,  // D
    SDL_SCANCODE_F,  // E
    SDL_SCANCODE_V   // F
};

int platform_input_init(void) {
    // SDL stuff already initiated in display.c
    
    return 0;
}

void platform_input_destroy(void) {
    // No teardown needed for SDL keeb

    return;
}

/* Update CHIP-8 keypad by polling SDL's keyboard state array */
void platform_input_update(uint8_t keypad[16]) {
    // Update event queue
    SDL_PumpEvents();

    // Pointer to an SDL-managed array of all scancodes
    const uint8_t *state = SDL_GetKeyboardState(NULL);

    // Check if any keys are currently pressed
    for (size_t i = 0; i < INPUT_KEY_COUNT; i++) {
        keypad[i] = state[keymap[i]] ? 1 : 0;
    }

    return;
}

#endif