#pragma once

#include <stdint.h>

#include "platform.h"

int platform_display_init(int scale);
void platform_display_destroy(void);
void platform_display_draw(const uint8_t *framebuffer);

int platform_display_poll_events(void);