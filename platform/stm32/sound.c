#include "sound.h"

#ifdef PLATFORM_STM32

#include "stm32f1xx_hal.h"

/*
    - Enabled Internal Clock
    - PSC: 7 => MHz tick
    - ARR: 499 => 2 KHz tick
*/

extern TIM_HandleTypeDef htim3; 

#define BUZZER_TIM htim3
#define BUZZER_CHANNEL TIM_CHANNEL_4  // PB1

/* Start PWM with 0% duty cycle */
int platform_sound_init(void) {
    if (HAL_TIM_PWM_Start(&BUZZER_TIM, BUZZER_CHANNEL) != HAL_OK) {
        return 1;
    }

    // Turn off
    __HAL_TIM_SET_COMPARE(&BUZZER_TIM, BUZZER_CHANNEL, 0);

    return 0;
}

void platform_sound_destroy(void) {
    HAL_TIM_PWM_Stop(&BUZZER_TIM, BUZZER_CHANNEL);

    return;
}

void platform_sound_update(int is_on) {
    if (is_on) {
        // 50% duty cycle (beep on)
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&BUZZER_TIM);

        __HAL_TIM_SET_COMPARE(&BUZZER_TIM, BUZZER_CHANNEL, arr / 2);
    } else {
        // 0% duty cycle (silent)
        __HAL_TIM_SET_COMPARE(&BUZZER_TIM, BUZZER_CHANNEL, 0);
    }
}

#endif