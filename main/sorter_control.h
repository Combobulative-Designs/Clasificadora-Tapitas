#include "common_stuff.h"
#include "sensor_control.h"
#include "servo_control.h"
#include "stepper_control.h"
#include "rgb_control.h"
#include "display_control.h"
//#include "encoder_control.h"

#ifndef GITHUB_REPO_SORTER_CONTROL_H
#define GITHUB_REPO_SORTER_CONTROL_H

class SorterControl {
public:
    SorterControl(SensorControl (&), StepperControl (&), ServoControl (&), RGBControl (&), DisplayControl (&));

    void processState();
    void initialize();


private:
    SensorControl &sensorControl;
    StepperControl &stepperControl;
    ServoControl &servoControl;
    RGBControl &rgbControl;
    DisplayControl &displayControl;

    bool initialized;

    void runStartSequence();
};

#endif //GITHUB_REPO_SORTER_CONTROL_H
