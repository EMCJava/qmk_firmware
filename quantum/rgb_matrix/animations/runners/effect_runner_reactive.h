#pragma once

#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED

extern bool emcjava_capslock_is_on;

typedef HSV (*reactive_f)(HSV hsv, uint16_t offset);

uint16_t tick_since_last_click(uint8_t index) {
    uint16_t max_tick = 65535 / qadd8(rgb_matrix_config.speed, 1);

    for (int8_t j = g_last_hit_tracker.count - 1; j >= 0; j--) {
        if (g_last_hit_tracker.index[j] == index && g_last_hit_tracker.tick[j] < max_tick) {
            return g_last_hit_tracker.tick[j];
        }
    }

    return max_tick;
}

bool effect_runner_reactive(effect_params_t* params, reactive_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint16_t tick = tick_since_last_click(i);

        HSV hsv = rgb_matrix_config.hsv;
        if(emcjava_capslock_is_on && i == 13)
            hsv.v = 0;

        uint16_t offset = scale16by8(tick, qadd8(rgb_matrix_config.speed, 1));
        RGB      rgb    = rgb_matrix_hsv_to_rgb(effect_func(hsv, offset));
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    // is fn layer
    if (biton32(layer_state) % 2 == 1) {
        static uint8_t button_indices_to_flash[4] = {71, 79, 80, 81};

        uint8_t time = sin8(scale16by8(g_rgb_timer, (fract16)7 * rgb_matrix_config.speed));

        for (uint8_t i = 0; i < 4; ++i) {
            uint16_t tick = tick_since_last_click(button_indices_to_flash[i]);
            uint16_t offset = scale16by8(tick, qadd8(rgb_matrix_config.speed, 1));
            HSV hsv = effect_func(rgb_matrix_config.hsv, offset);
            hsv.v   = time;
            RGB rgb = rgb_matrix_hsv_to_rgb(hsv);

            rgb_matrix_set_color(button_indices_to_flash[i], rgb.r, rgb.g, rgb.b);
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED
