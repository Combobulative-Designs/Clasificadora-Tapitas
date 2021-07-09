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
    lastActivity(millis()),
    auxLEDState(LOW),
    auxLEDStateChanged(true),
    queuedReadAction(false),
    sensor(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X),
    emptyReading{0,0,0},
    lastReading{0,0,0},
    activeSamplesArchive(0)
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

RGBColor SensorControl::getRGB() {
    return lastReading;
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

void SensorControl::getRGBStr(char* rgbStr) {
    if (initialized) {
        for (int i = 0; i < 12; i++) {
            rgbStr[i] = '\0';
        }

        char channelStr[5];
        itoa(lastReading.red, channelStr, 10);
        strcat(rgbStr, channelStr);
        strcat(rgbStr, (char *) ",");
        itoa(lastReading.green, channelStr, 10);
        strcat(rgbStr, channelStr);
        strcat(rgbStr, (char *) ",");
        itoa(lastReading.blue, channelStr, 10);
        strcat(rgbStr, channelStr);
    }
}


void SensorControl::getRGBName(char* nameStr) {
    if (initialized) {
        int colorDistMin = 9999;
        int colorDist;
        int closestSampleIndex;

        for (int i = 0; i < strlen(nameStr); i++) {
            nameStr[i] = '\0';
        }

        RGBSampleSet sampleSet;
        PROGMEM_readAnything (&samplesArchive [activeSamplesArchive], sampleSet);
        for (int i = 0; i < 12; i++) {
            colorDist = getDistanceToSample(lastReading, sampleSet.colors[i]);
            if (colorDist < colorDistMin) {
                closestSampleIndex = i;
                colorDistMin = colorDist;
            }
        }
        
        char name[11];
        PROGMEM_readAnything (&sampleNames [closestSampleIndex], name);
        Serial.println(closestSampleIndex);
        Serial.println(activeSamplesArchive);
        strcat(nameStr, name);
    }
}

void SensorControl::getRGBStr(char* rgbStr, RGBColor rgbColor) {
    if (initialized) {
        for (int i = 0; i < 12; i++) {
            rgbStr[i] = '\0';
        }

        char channelStr[5];
        itoa(rgbColor.red, channelStr, 10);
        strcat(rgbStr, channelStr);
        strcat(rgbStr, (char *) ",");
        itoa(rgbColor.green, channelStr, 10);
        strcat(rgbStr, channelStr);
        strcat(rgbStr, (char *) ",");
        itoa(rgbColor.blue, channelStr, 10);
        strcat(rgbStr, channelStr);
    }
}


void SensorControl::getRGBName(char* nameStr, RGBColor rgbColor) {
    if (initialized) {
        int colorDistMin = 9999;
        int colorDist;
        int closestSampleIndex;

        for (int i = 0; i < strlen(nameStr); i++) {
            nameStr[i] = '\0';
        }

        RGBSampleSet sampleSet;
        PROGMEM_readAnything (&samplesArchive [activeSamplesArchive], sampleSet);
        for (int i = 0; i < 12; i++) {
            colorDist = getDistanceToSample(lastReading, sampleSet.colors[i]);
            if (colorDist < colorDistMin) {
                closestSampleIndex = i;
                colorDistMin = colorDist;
            }
        }

        char name[11];
        PROGMEM_readAnything (&sampleNames [closestSampleIndex], name);
        strcat(nameStr, name);
    }
}

enum ColorCategory SensorControl::getColorCategory() {
    if (initialized) {
        int colorDistMin = 9999;
        int colorDist;
        int closestSampleIndex;

        RGBSampleSet sampleSet;
        PROGMEM_readAnything(&samplesArchive[activeSamplesArchive], sampleSet);
        for (int i = 0; i < 12; i++) {
            colorDist = getDistanceToSample(lastReading, sampleSet.colors[i]);
            if (colorDist < colorDistMin) {
                closestSampleIndex = i;
                colorDistMin = colorDist;
            }
        }

        return RGBColorSampleIndexToCategory(closestSampleIndex);
    }
}

void SensorControl::setActiveSampleArchive(int archiveIndex) {
    if (initialized) {
        activeSamplesArchive = archiveIndex;
    }
}

void SensorControl::readColor() {
    if (initialized) {
        uint16_t clearReading = sensor.read16(TCS34725_CDATAL);

        lastReading.red = (int)(((float)sensor.read16(TCS34725_RDATAL) / clearReading) * 255.0);
        lastReading.green = (int)(((float)sensor.read16(TCS34725_GDATAL) / clearReading) * 255.0);
        lastReading.blue = (int)(((float)sensor.read16(TCS34725_BDATAL) / clearReading) * 255.0);
    }
}

double SensorControl::getDistanceToSample(RGBColor reading, RGBColor sample) {
    //return sqrt(pow(reading.red - sample.red, 2) + pow(reading.green - sample.green, 2) + pow(reading.blue - sample.blue, 2));
    long rmean = ( (long)reading.red + (long)sample.red ) / 2;
    long r = (long)reading.red - (long)sample.red;
    long g = (long)reading.green - (long)sample.green;
    long b = (long)reading.blue - (long)sample.blue;
    return sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8));
}
