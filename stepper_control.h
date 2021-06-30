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
    /*
    int stepCounter;
    int coilStepCounter;
    */

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

    /*
    const int stepsTotal = 4076;
    const int numCoilSteps = 8;
    const int coilStepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };
    const int positionsSteps[12] = {0, 340, 680, 1019, 1359, 1698, 2038, 2378, 2718, 3057, 3397, 3736};
    */

    /* Estructura de pasos
     * ===================
     *
     * 340
     * 340
     * 339
     * 340
     * 339
     * 340
     * 340
     * 340
     * 339
     * 340
     * 339
     * 340
     *
     * Total 4076
     * 12 posiciones
     */

    /*
    void rotateStep(enum RotationDirection direction);
    void writeToStepper(int step);
    */
};

#endif //CLASIFICADORA_STEPPER_CONTROL_H
