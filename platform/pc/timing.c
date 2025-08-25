#include "timing.h"

#ifdef PLATFORM_PC

#include <stdint.h>
#include <SDL2/SDL.h>

#include "timers.h"

static uint32_t frame_delay = 0;
static uint32_t frame_start = 0;
static uint32_t last_timer_update = 0;

int platform_timing_init(int cycle_delay_ms) {
    if (cycle_delay_ms <= 0) {
        return 1;
    }

    frame_delay = (uint32_t)cycle_delay_ms;

    // Capture initial time
    frame_start = SDL_GetTicks();

    // Align 60 Hz timers
    last_timer_update = frame_start;

    return 0;
}

void platform_timing_destroy(void) {
    // Not neccessary with SDL2

    return;
}

/* Delay so the next CPU cycle starts after `frame_delay` ms, then decrement CHIP-8 timers at true 60 Hz. */
void platform_timing_sync(Chip8* chip8) {
    uint32_t now = SDL_GetTicks();
    uint32_t elapsed = now - frame_start;

    if (elapsed < frame_delay) {
        // Wait out remaining cycle time
        SDL_Delay(frame_delay - elapsed);
        now = SDL_GetTicks();
    }

    // Start of next cycle window
    frame_start = now; 

    // Tick timers at 60 Hz (and catch up if it slipped up)
    const uint32_t step = 1000 / TIMER_CLOCK_FREQUENCY_HZ;  // 16 ms

    while (now - last_timer_update >= step) {
        timers_tick(&chip8->delay_timer, &chip8->sound_timer);
        last_timer_update += step;
    }

    return;
}

#endif