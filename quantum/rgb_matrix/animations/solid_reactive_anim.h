#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
#    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE
RGB_MATRIX_EFFECT(SOLID_REACTIVE)
#        ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static HSV SOLID_REACTIVE_math(HSV hsv, uint16_t offset) {
    // hsv.h = addmod8(hsv.h, qsub8(255, offset) >> 2, 255);
    // hsv.h = qsub8(hsv.h, qsub8(255, offset));
    hsv.h = scale8(qsub8(255, qmul8(qsub8(255, offset), 2)), hsv.h);
    // hsv.h = scale8(qsub8(255, qsub8(cos8(scale8(90, offset)), 128) << 1), hsv.h);
    return hsv;
}

bool SOLID_REACTIVE(effect_params_t* params) { return effect_runner_reactive(params, &SOLID_REACTIVE_math); }

#        endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#    endif      // ENABLE_RGB_MATRIX_SOLID_REACTIVE
#endif          // RGB_MATRIX_KEYREACTIVE_ENABLED
