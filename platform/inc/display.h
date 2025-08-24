#pragma once

#include <stdint.h>

#include "platform.h"

#define SCALE 10
#define INSTRUCTIONS_PER_FRAME 10

int platform_display_init(void);
void platform_display_destroy(void);
void platform_display_draw(const uint8_t *framebuffer);

int platform_display_poll_events(void);