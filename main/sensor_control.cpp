#include <Arduino.h>
#include "Adafruit_TCS34725.h"

#include "common_stuff.h"

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
    queuedReadAction(false),
    sensor(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X)
    {}

void SensorControl::initialize() {
    if (!initialized) {
        sensor.begin();
        sensor.setInterrupt(true);
        sensor.write8(TCS34725_PERS, TCS34725_PERS_NONE);
        pinMode(auxLEDPin, OUTPUT);
        if (auxLEDMode == SensorAuxLEDMode::Manual) {
            auxLEDState = HIGH;
        } else auxLEDState = LOW;
        initialized = true;
    }
    //COLOR_init();
}

void SensorControl::setAuxLEDMode(enum SensorAuxLEDMode p_mode) {
    if (initialized) {
        auxLEDMode = p_mode;
    }
}

bool SensorControl::isBusy() {
    if (initialized) {
        return busy;
    } else {
        return false;
    }
}

enum ColorCategory SensorControl::getColorRead() {
    return colorRead;
}

RGBColor SensorControl::getColorReadRGB() {
    return colorReadRGB;
}

void SensorControl::requestColorReading() {
    if (initialized) {
        queuedReadAction = true;
    }
}

void SensorControl::processState() {
    if (initialized) {
        if (auxLEDStateChanged) {
            if (auxLEDState == HIGH) {
                sensor.setInterrupt(false);
            }
            else {
                sensor.setInterrupt(true);
            }
            //digitalWrite(auxLEDPin, auxLEDState);
            auxLEDStateChanged = false;
        }

        if (!busy) {
            if (queuedReadAction) {
                if (auxLEDMode == SensorAuxLEDMode::Auto) {
                    auxLEDState = HIGH;
                    auxLEDStateChanged = true;
                    busy = true;
                } else {
                    if (millis() - lastActivity >= 60) {
                        readColor();
                        queuedReadAction = false;
                    }
                }
                lastActivity = millis();
            }
        } else {
            if (auxLEDMode == SensorAuxLEDMode::Auto) {
                if (millis() - lastActivity >= 20) {
                    if (queuedReadAction and auxLEDState == HIGH) {
                        readColor();
                        queuedReadAction = false;
                        lastActivity = millis();
                    } else if (!queuedReadAction and auxLEDState == HIGH) {
                        if (millis() - lastActivity >= 700) {
                            auxLEDState = LOW;
                            auxLEDStateChanged = true;
                            lastActivity = millis();
                        }
                    } else if (!queuedReadAction and auxLEDState == LOW) {
                        lastActivity = millis();
                        busy = false;
                    }
                }
            }
        }
    }
}

void SensorControl::readColor() {
    if (initialized) {
        colorReadRGB.white = sensor.read16(TCS34725_CDATAL);
        colorReadRGB.red = sensor.read16(TCS34725_RDATAL);
        colorReadRGB.green = sensor.read16(TCS34725_GDATAL);
        colorReadRGB.blue = sensor.read16(TCS34725_BDATAL);

        int distance;
        int minDistance = 999;
        int index;

        for (byte i = 0; i < 7; ++i) {
            distance = sqrt(pow(colorReadRGB.red - previousSamples[i][0], 2) + pow(colorReadRGB.green - previousSamples[i][1], 2) + pow(colorReadRGB.blue - previousSamples[i][2], 2));
            if (distance < minDistance) {
                index = i;
                minDistance = distance;
            }
        }
        colorRead = (enum ColorCategory)index;
        Serial.print((((float)colorReadRGB.red / colorReadRGB.white) * 255.0), DEC);
        Serial.print(" - ");
        Serial.print((((float)colorReadRGB.blue / colorReadRGB.white) * 255.0), DEC);
        Serial.print(" - ");
        Serial.print((((float)colorReadRGB.green / colorReadRGB.white) * 255.0), DEC);
        Serial.print(" - ");
        Serial.println(colorReadRGB.white, DEC);
    }









/*
    colorReadRGB = COLOR_getRGB()
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
    }*/
}
