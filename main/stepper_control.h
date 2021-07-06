#include <AccelStepper.h>

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
    bool isBusy();

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

    int stepSize;
    int stepMax;

    //const int speed = 500;
    //const int totalSteps = 4096;
    //const int positions = 12;
    const int positionSteps[8] = {
            0,
            512,
            1024,
            1536,
            2048,
            2560,
            3072,
            3584
    };
};

#endif //CLASIFICADORA_STEPPER_CONTROL_H
