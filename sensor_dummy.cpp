#include <Arduino.h>

#include "common_stuff.h"

#include "COLOR.h"
#include "sensor_dummy.h"

const enum ColorCategory colors[7] = {
        ColorCategory::Reds, ColorCategory::Yellows, ColorCategory::Blues,
        ColorCategory::Greens, ColorCategory::Whites, ColorCategory::Greys,
        ColorCategory::Blacks
};

enum ColorCategory ReadColorDummy() {
    int randColor = random(0,7);
    Serial.print("Reading color (DUMMY): ");
    Serial.println(randColor);
    return colors[randColor];
}

void InitSensor() {
    COLOR_H::COLOR_init();
}

enum ColorCategory ReadColor() {
    Serial.println("Reading color (REAL):");
    RGB_t colorRGB = COLOR_getRGB();
    HSL_t colorHSL = COLOR_getHSLfromRGB(colorRGB);
    double saturation = colorHSL.saturation;
    double luminosity = colorHSL.luminosity;
    double hue = colorHSL.hue;

    enum ColorCategory category;

    if (luminosity >= ALT_LUMINOSITY_MIN_BLANCO && saturation < ALT_SATURATION_MAX_BLANCO) {
        category = ColorCategory::Whites;
        Serial.println("WHITE.");
    } else if (luminosity < ALT_LUMINOSITY_MAX_NEGRO && saturation < ALT_SATURATION_MAX_NEGRO) {
        category = ColorCategory::Blacks;
        Serial.println("BLACK.");
    } else if (luminosity < ALT_LUMINOSITY_MAX_GRIS && saturation < ALT_SATURATION_MAX_GRIS) {
        category = ColorCategory::Greys;
        Serial.println("GREY.");
    } else {
        if (hue < ALT_MAX_HUE_ROJO) {
            category = ColorCategory::Reds;
            Serial.println("RED.");
        } else if (hue < ALT_MAX_HUE_NARANJA) {
            category = ColorCategory::Reds;
            Serial.println("RED.");
        } else if (hue < ALT_MAX_HUE_AMARILLO) {
            category = ColorCategory::Yellows;
            Serial.println("YELLOW.");
        } else if (hue < ALT_MAX_HUE_VERDE) {
            category = ColorCategory::Greens;
            Serial.println("GREEN.");
        } else if (hue < ALT_MAX_HUE_AZUL) {
            category = ColorCategory::Blues;
            Serial.println("BLUE.");
        } else {
            category = ColorCategory::Reds;
            Serial.println("RED.");
        }
    }

    return category;
}