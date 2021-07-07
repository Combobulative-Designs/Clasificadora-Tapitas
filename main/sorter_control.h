#include "common_stuff.h"
#include "sensor_control.h"
#include "servo_control.h"
#include "stepper_control.h"
#include "simple_output_control.h"
#include "rgb_control.h"
#include "display_control.h"
//#include "encoder_control.h"

#ifndef GITHUB_REPO_SORTER_CONTROL_H
#define GITHUB_REPO_SORTER_CONTROL_H

class SorterControl {
public:
    SorterControl(SensorControl (&), StepperControl (&), ServoControl (&), SOutputControl (&), RGBControl (&), DisplayControl (&));

    void processState();
    void initialize();
    void startProgram(enum SorterPrograms);
    void stopProgram();

    bool isBusy();

private:
    SensorControl &sensorControl;
    StepperControl &stepperControl;
    ServoControl &servoControl;
    SOutputControl &sOutputControl;
    RGBControl &rgbControl;
    DisplayControl &displayControl;


    bool initialized;
    bool locked;
    bool stopSignal;
    enum SorterActions lastAction;
    enum SorterPrograms currentProgram;
    int currentProgramStep;
    unsigned long programStartTime;
    enum ColorCategory colorCategoryRead;
    unsigned long autoOperationTime;
    int processedTotal;
    int processedByColor[7];

    void PGM_Startup();
    void PGM_Automatic();

    void PGM_ManualStepper();
    void PGM_ManualServo();
    void PGM_ManualSensor();

    void PGM_ByStepNext();

    void PGM_CycleStepper();
    void PGM_SensorData();
};

#endif //GITHUB_REPO_SORTER_CONTROL_H
