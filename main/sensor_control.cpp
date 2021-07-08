#include <Arduino.h>
#include "Adafruit_TCS34725.h"

#include "common_stuff.h"
#include "progmem_data.h"
#include "PROGMEM_readAnything.h"

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
    }
}

int SensorControl::getNearestSampleIndex() {
    if (initialized) {
        RGBColorNorm rgbColorNorm = NormalizeRGBColor(colorReadRGB);
        int colorDistMin = 9999;
        int colorDist;
        int closestSampleIndex;

        for (int i = 0; i < 11; i++) {
            RGBColorNorm sample = {0,0,0};
            PROGMEM_readAnything (&samples [i], sample);
            colorDist = calcColorDistance(rgbColorNorm, sample);
            if (colorDist < colorDistMin) {
                closestSampleIndex = i;
                colorDistMin = colorDist;
            }
        }

        return closestSampleIndex;
    } else {
        return 0;
    }
}

double SensorControl::calcColorDistance(RGBColorNorm reading, RGBColorNorm sample) {
    //return sqrt(pow(reading.red - sample.red, 2) + pow(reading.green - sample.green, 2) + pow(reading.blue - sample.blue, 2));
    long rmean = ( (long)reading.red + (long)sample.red ) / 2;
    long r = (long)reading.red - (long)sample.red;
    long g = (long)reading.green - (long)sample.green;
    long b = (long)reading.blue - (long)sample.blue;
    return sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8));
}
