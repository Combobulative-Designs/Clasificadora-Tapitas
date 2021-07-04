#include <Arduino.h>

#include "sorter_control.h"

#include "common_stuff.h"
#include "sensor_control.h"
#include "servo_control.h"
#include "stepper_control.h"
#include "rgb_control.h"
#include "display_control.h"

SorterControl::SorterControl(SensorControl &p_sensorControl,
                             StepperControl &p_stepperControl,
                             ServoControl &p_servoControl,
                             RGBControl &p_rgbControl,
                             DisplayControl &p_displayControl
                             ) :
                             sensorControl(p_sensorControl),
                             stepperControl(p_stepperControl),
                             servoControl(p_servoControl),
                             rgbControl(p_rgbControl),
                             displayControl(p_displayControl)
                             {}

void SorterControl::initialize() {
    if (!initialized) {
        sensorControl.initialize();
        displayControl.initialize();
        stepperControl.initialize();
        servoControl.initialize();
        rgbControl.initialize();

        runStartSequence();

    }
}




void SorterControl::runStartSequence() {
    displayControl.noNavArrows();
    displayControl.setLineText(
            (char*)"TAPITANATOR 9KPP",
            0,
            TextAlignment::Center);
    displayControl.setLineText(
            (char*)"¡Bienvenido!",
            1,
            TextAlignment::Center);

    initialized = true;
}