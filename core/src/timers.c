#include "timers.h"

/* Prepare timer registers */
void timers_init(Chip8 *chip8) {
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    return;
}

/* Update timers at 60 Hz */
void timers_tick(uint8_t *delay_timer, uint8_t *sound_timer) {
    if (*delay_timer > 0) {
        --*delay_timer;
    }

    if (*sound_timer > 0) {
        --*sound_timer;
    }

    return;
}