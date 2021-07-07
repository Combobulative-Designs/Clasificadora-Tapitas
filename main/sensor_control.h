#include "COLOR.h"
#include "Adafruit_TCS34725.h"

#include "common_stuff.h"

#ifndef CLASIFICADORA_TI3_SENSOR_CONTROL_H
#define CLASIFICADORA_TI3_SENSOR_CONTROL_H

const int previousSamples[7][3] = {
        {2062, 1469,  861},
        {1735,  758,  502},
        {1148,  440,  415},
        {1226,  478,  520},
        { 907,  580,  620},
        {1837, 1529, 1437},
        { 825,  821,  971}
};

class SensorControl {
public:
    SensorControl(int p_pin_aux_led);

    void initialize();
    void processState();

    void setAuxLEDMode(enum SensorAuxLEDMode);
    bool isBusy();
    enum ColorCategory getColorRead();
    RGBColor getColorReadRGB();
    void requestColorReading();

private:
    enum SensorAuxLEDMode auxLEDMode;
    enum ColorCategory colorRead;
    Adafruit_TCS34725 sensor;
    RGBColor colorReadRGB;
    int auxLEDPin;
    bool initialized;
    bool busy;
    bool auxLEDState;
    bool auxLEDStateChanged;
    unsigned long lastActivity;
    bool queuedReadAction;

    void readColor();
};

#endif //CLASIFICADORA_TI3_SENSOR_CONTROL_H
