#include "display.h"

#ifdef PLATFORM_PC

#include <SDL2/SDL.h>

#include "chip8.h"

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture  *texture;
} SdlConfig;

static SdlConfig sdl_config = {NULL, NULL, NULL};

int display_init(void) {
    // Set up SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL video subsystem could not be initialized: %s\n", SDL_GetError());

        return 1;
    }

    // Create scaled-up window (currently from 64x32 to 640x320)
    sdl_config.window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE,
        SDL_WINDOW_SHOWN
    );

    if (!sdl_config.window) {
        SDL_Log("Window could not be created: %s\n", SDL_GetError());

        return 1;
    }

    // Create accelerated renderer
    sdl_config.renderer = SDL_CreateRenderer(sdl_config.window, -1, SDL_RENDERER_ACCELERATED);

    if (!sdl_config.renderer) {
        SDL_Log("Renderer could not be created: %s\n", SDL_GetError());

        return 1;
    }

    // Create texture (64x32 pixel buffer, RGBA)
    sdl_config.texture = SDL_CreateTexture(
        sdl_config.renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT
    );
    if (!sdl_config.texture) {
        SDL_Log("Texture could not be created: %s\n", SDL_GetError());
         
        return 1;
    }

    return 0;
}

void display_destroy(void) {

}

void display_draw(const Chip8* chip8) {

}

#endif