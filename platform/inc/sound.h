#pragma once

#include "platform.h"

int platform_sound_init(void);
void platform_sound_destroy(void);
void platform_sound_update(int is_on);