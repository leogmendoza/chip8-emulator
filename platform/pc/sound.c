#include "sound.h"

#ifdef PLATFORM_PC

#include <stdint.h>
#include <SDL2/SDL.h>

// 800 Hz beep, 16-bit mono
#define TONE_HZ 800
#define AMP_16BIT 3000

typedef struct {
    SDL_AudioDeviceID device;
    SDL_AudioSpec     spec;
    int               playing;
    float             phase; // [0,1) phase accumulator
} SdlAudioConfig;

static SdlAudioConfig sdl_audio_config = {0};

/* SDL audio callback, which outputs square wave or silence */
static void SDLCALL audio_cb(void *userdata, Uint8 *stream, int len_bytes) {
    // Silence unused param warninga
    (void)userdata;

    int16_t *out = (int16_t *)stream;
    int frames = len_bytes / (int)sizeof(int16_t);

    if (!sdl_audio_config.device || sdl_audio_config.spec.freq <= 0 || !sdl_audio_config.playing) {
        SDL_memset(stream, 0, len_bytes);

        return;
    }

    const float step = (float)TONE_HZ / (float)sdl_audio_config.spec.freq;

    for (int i = 0; i < frames; ++i) {
        sdl_audio_config.phase += step;

        if (sdl_audio_config.phase >= 1.0f) {
            sdl_audio_config.phase -= 1.0f;
        }

        out[i] = (sdl_audio_config.phase < 0.5f) ? AMP_16BIT : -AMP_16BIT;
    }

    return;
}

/* Set up SDL audio subsystem and start playback thread */
int platform_sound_init(void) {
    if (( SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO) == 0 ) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            SDL_Log("SDL audio init failed: %s\n", SDL_GetError());

            return 1;
        }
    }

    SDL_AudioSpec want;
    SDL_zero(want);

    want.freq     = 48000;
    want.format   = AUDIO_S16SYS;
    want.channels = 1;
    want.samples  = 512;
    want.callback = audio_cb;

    sdl_audio_config.device = SDL_OpenAudioDevice(NULL, 0, &want, &sdl_audio_config.spec, 0);

    if (sdl_audio_config.device == 0) {
        SDL_Log("SDL_OpenAudioDevice failed: %s\n", SDL_GetError());

        return 1;
    }

    sdl_audio_config.playing = 0;
    sdl_audio_config.phase   = 0.0f;

    SDL_PauseAudioDevice(sdl_audio_config.device, 0);

    return 0;
}

void platform_sound_destroy(void) {
    if (sdl_audio_config.device) {
        SDL_CloseAudioDevice(sdl_audio_config.device);
        sdl_audio_config.device = 0;
    }

    return;
}

/* Update playing flag based on CHIP-8 sound timer */
void platform_sound_update(int is_on) {
    if (!sdl_audio_config.device) {
        return;
    }

    SDL_LockAudioDevice(sdl_audio_config.device);
    sdl_audio_config.playing = (is_on != 0);
    SDL_UnlockAudioDevice(sdl_audio_config.device);

    return;
}

#endif