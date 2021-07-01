#include <AccelStepper.h>
#include <MultiStepper.h>

#include "common_stuff.h"

#ifndef CLASIFICADORA_STEPPER_CONTROL_H
#define CLASIFICADORA_STEPPER_CONTROL_H

enum class StepperActions {Cycling, CapStep, Resting, Stopping};

class StepperControl {
public:
    StepperControl(int p_pinIN1, int p_pinIN2, int p_pinIN3, int p_pinIN4);

    void initialize();
    void processState();
    void doCycling();
    void doCapStep();
    void stopActions();
    StepperActions getCurrentAction();

private:
    bool initialized;

    AccelStepper stepper;

    int pinIN1;
    int pinIN2;
    int pinIN3;
    int pinIN4;

    int positionCounter;
    int nextPosition;
    bool actionChange;
    enum StepperActions currentAction;
    enum StepperActions previousAction;

    const int speed = 500;
    const int totalSteps = 4096;
    const int positions = 12;
    const int positionSteps[12] = {
            0,
            341,
            682,
            1023,
            1364,
            1705,
            2046,
            2387,
            2728,
            3069,
            3410,
            3755
    };
};

#endif //CLASIFICADORA_STEPPER_CONTROL_H
