#include <AccelStepper.h>

#include <Arduino.h>

#include "common_stuff.h"

#include "stepper_control.h"

StepperControl::StepperControl(int p_pinIN1, int p_pinIN2, int p_pinIN3, int p_pinIN4) :
    initialized(false),
    positionCounter(0),
    nextPosition(0),
    actionChange(false),
    currentAction(StepperActions::Resting),
    previousAction(StepperActions::Resting),
    stepper(8, p_pinIN1, p_pinIN3, p_pinIN2, p_pinIN4)
    {}

StepperActions StepperControl::getCurrentAction() {return currentAction;}

void StepperControl::initialize() {
    if (!initialized) {
        stepper.setCurrentPosition(0);
        stepper.setMaxSpeed(500);

        initialized = true;
    }
}

void StepperControl::processState() {
    if (initialized) {
        if (stepper.currentPosition() >= 4096) stepper.setCurrentPosition(stepper.currentPosition() - 4096);
      
        switch (currentAction) {
            case StepperActions::Resting:
                if (actionChange) {
                    actionChange = false;
                }
                break;
            case StepperActions::CapStep:
                if (actionChange) {
                    actionChange = false;
                    nextPosition = positionCounter + 1;
                    if (nextPosition >= 12) { nextPosition = 0; }
                    stepper.enableOutputs();
                    stepper.setSpeed(500);
                    stepper.moveTo(positionSteps[nextPosition]);
                }
                if (stepper.currentPosition() == stepper.targetPosition()) {
                    positionCounter = nextPosition;
                    actionChange = true;
                    currentAction = StepperActions::Stopping;
                } else {
                    stepper.setSpeed(500);
                    stepper.runSpeed();
                }
                break;
            case StepperActions::Cycling:
                if (actionChange) {
                    actionChange = false;
                    nextPosition = positionCounter + 1;
                    if (nextPosition >= 12) { nextPosition = 0; }
                    stepper.enableOutputs();
                    stepper.setSpeed(500);
                    stepper.moveTo(positionSteps[nextPosition]);
                }
                if (stepper.currentPosition() == stepper.targetPosition()) {
                    positionCounter = nextPosition;
                    nextPosition = positionCounter + 1;
                    if (nextPosition >= 12) { nextPosition = 0; }
                    stepper.moveTo(positionSteps[nextPosition]);
                    stepper.setSpeed(500);
                    stepper.runSpeed();
                } else {
                    stepper.setSpeed(500);
                    stepper.runSpeed();
                }
                break;
            case StepperActions::Stopping:
                if (actionChange) {
                    actionChange = false;
                    stepper.disableOutputs();
                    if (previousAction == StepperActions::Cycling) {
                        stepper.setCurrentPosition(0);
                        positionCounter = 0;
                    }
                    actionChange = true;
                    currentAction = StepperActions::Resting;
                }
                break;
        }

        previousAction = currentAction;
    }
}

void StepperControl::doCycling() {
    if (initialized) {
        if (currentAction == StepperActions::Resting) {
            actionChange = true;
            currentAction = StepperActions::Cycling;
        }
    }
}

void StepperControl::doCapStep() {
    if (initialized) {
        if (currentAction == StepperActions::Resting) {
            actionChange = true;
            currentAction = StepperActions::CapStep;
        }
    }
}
void StepperControl::stopActions() {
    if (initialized) {
        if (currentAction == StepperActions::CapStep || currentAction == StepperActions::Cycling) {
            actionChange = true;
            currentAction = StepperActions::Stopping;
        }
    }
}

bool StepperControl::isBusy() {
    if (initialized) {
        if (currentAction == StepperActions::Resting) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}
