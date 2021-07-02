#include <Arduino.h>

#include "common_stuff.h"

#include "COLOR.h"
#include "sensor_dummy.h"

void InitSensor() {
    COLOR_H::COLOR_init();
}

enum ColorCategory ReadColor() {
    HSL_t colorHSL = COLOR_getHSLfromRGB(COLOR_getRGB());

    if (colorHSL.luminosity >= LUMINOSITY_MIN_BLANCO && colorHSL.saturation < SATURATION_MAX_BLANCO) {
        return ColorCategory::Whites;
    } else if (colorHSL.luminosity < LUMINOSITY_MAX_NEGRO && colorHSL.saturation < SATURATION_MAX_NEGRO) {
        return ColorCategory::Blacks;
    } else if (colorHSL.luminosity < LUMINOSITY_MAX_GRIS && colorHSL.saturation < SATURATION_MAX_GRIS) {
        return ColorCategory::Greys;
    } else {
        if (colorHSL.hue < MAX_HUE_ROJO) {
            return ColorCategory::Reds;
        } else if (colorHSL.hue < MAX_HUE_NARANJA) {
            return ColorCategory::Reds;
        } else if (colorHSL.hue < MAX_HUE_AMARILLO) {
            return ColorCategory::Yellows;
        } else if (colorHSL.hue < MAX_HUE_VERDE) {
            return ColorCategory::Greens;
        } else if (colorHSL.hue < MAX_HUE_AZUL) {
            return ColorCategory::Blues;
        } else {
            return ColorCategory::Reds;
        }
    }
}
