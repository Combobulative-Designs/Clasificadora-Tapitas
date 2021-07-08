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
    bool isBusy();

    RGBColor getColorReadRGB();
    int getNearestSampleIndex();

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

    double calcColorDistance(RGBColorNorm, RGBColorNorm);
    void readColor();
    void readColorRGB();
};

#endif //CLASIFICADORA_TI3_SENSOR_CONTROL_H
