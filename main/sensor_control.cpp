#include <Arduino.h>

#include "common_stuff.h"

#include "COLOR.h"
#include "sensor_control.h"

SensorControl::SensorControl(int p_pin_aux_led) :
    initialized(false),
    auxLEDPin(p_pin_aux_led),
    auxLEDMode(SensorAuxLEDMode::Auto),
    busy(false),
    colorRead(ColorCategory::Blacks),
    lastActivity(millis()),
    auxLEDState(LOW),
    auxLEDStateChanged(true),
    queuedReadAction(false)
    {}

void SensorControl::initialize() {
    COLOR_init();
    pinMode(auxLEDPin, OUTPUT);
    if (auxLEDMode == SensorAuxLEDMode::Manual) {
        auxLEDState = HIGH;
    } else auxLEDState = LOW;
}

void SensorControl::setAuxLEDMode(enum SensorAuxLEDMode p_mode) {
    auxLEDMode = p_mode;
}

bool SensorControl::isBusy() {
    return busy;
}

enum ColorCategory SensorControl::getColorRead() {
    Serial.println((int)colorRead);
    return colorRead;
}

void SensorControl::requestColorReading() {
    queuedReadAction = true;
}

void SensorControl::processState() {
    if (auxLEDStateChanged) {
        digitalWrite(auxLEDPin, auxLEDState);
        auxLEDStateChanged = false;
    }

    if (!busy) {
        if (queuedReadAction) {
            if (auxLEDMode == SensorAuxLEDMode::Auto) {
                auxLEDState = HIGH;
                auxLEDStateChanged = true;
                busy = true;
            } else {
                readColor();
                queuedReadAction = false;
            }
            lastActivity = millis();
        }
    } else {
        if (auxLEDMode == SensorAuxLEDMode::Auto) {
            if (millis() - lastActivity >= 20) {
                if (queuedReadAction and auxLEDState == HIGH) {
                    readColor();
                    queuedReadAction = false;
                } else if (!queuedReadAction and auxLEDState == HIGH) {
                    auxLEDState = LOW;
                    auxLEDStateChanged = true;
                } else if (!queuedReadAction and auxLEDState == LOW) {
                    busy = false;
                }
                lastActivity = millis();
            }
        }
    }
}

void SensorControl::readColor() {
    HSL_t colorHSL = COLOR_getHSLfromRGB(COLOR_getRGB());

    if (colorHSL.luminosity >= LUMINOSITY_MIN_BLANCO && colorHSL.saturation < SATURATION_MAX_BLANCO) {
        colorRead = ColorCategory::Whites;
    } else if (colorHSL.luminosity < LUMINOSITY_MAX_NEGRO && colorHSL.saturation < SATURATION_MAX_NEGRO) {
        colorRead = ColorCategory::Blacks;
    } else if (colorHSL.luminosity < LUMINOSITY_MAX_GRIS && colorHSL.saturation < SATURATION_MAX_GRIS) {
        colorRead = ColorCategory::Greys;
    } else {
        if (colorHSL.hue < MAX_HUE_ROJO) {
            colorRead = ColorCategory::Reds;
        } else if (colorHSL.hue < MAX_HUE_NARANJA) {
            colorRead = ColorCategory::Reds;
        } else if (colorHSL.hue < MAX_HUE_AMARILLO) {
            colorRead = ColorCategory::Yellows;
        } else if (colorHSL.hue < MAX_HUE_VERDE) {
            colorRead = ColorCategory::Greens;
        } else if (colorHSL.hue < MAX_HUE_AZUL) {
            colorRead = ColorCategory::Blues;
        } else {
            colorRead = ColorCategory::Reds;
        }
    }
}
