#include "common_stuff.h"

#ifndef CLASIFICADORA_TI3_SENSOR_CONTROL_H
#define CLASIFICADORA_TI3_SENSOR_CONTROL_H

class SensorControl {
public:
    SensorControl(int);

    void initialize();
    void processState();

    void setAuxLEDMode(enum SensorAuxLEDMode);
    bool isBusy();
    enum ColorCategory getColorRead();
    void requestColorReading();

private:
    enum SensorAuxLEDMode auxLEDMode;
    enum ColorCategory colorRead;
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
