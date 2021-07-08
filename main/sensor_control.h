#include "COLOR.h"
#include "Adafruit_TCS34725.h"

#include "common_stuff.h"

#ifndef CLASIFICADORA_TI3_SENSOR_CONTROL_H
#define CLASIFICADORA_TI3_SENSOR_CONTROL_H

class SensorControl {
public:
    SensorControl(int p_pin_aux_led);

    void initialize();
    void processState();
    void setAuxLEDMode(enum SensorAuxLEDMode);
    void requestColorReading();
    void setEmptyReading();
    void getRGBStr(char*);
    void getRGBName(char*);
    void getRGBStr(char*, RGBColor);
    void getRGBName(char*, RGBColor);
    int getNearestSampleIndex();
    void setActiveSampleArchive(int);
    double getDistanceToSample(RGBColor, RGBColor);
    bool isBusy();
    RGBColor getRGB();


private:
    Adafruit_TCS34725 sensor;
    int auxLEDPin;
    int activeSamplesArchive;
    bool initialized;
    bool busy;
    bool auxLEDState;
    bool auxLEDStateChanged;
    bool queuedReadAction;
    RGBColor emptyReading;
    RGBColor lastReading;
    enum SensorAuxLEDMode auxLEDMode;
    unsigned long lastActivity;

    void readColor();
};

#endif //CLASIFICADORA_TI3_SENSOR_CONTROL_H
