#include <Arduino.h>

#include "common_stuff.h"

#include "stepper_control.h"

StepperControl::StepperControl() : initialized(false), stepCounter(0), positionCounter(0), actionChange(false), currentAction(StepperActions::Resting) {}

StepperActions StepperControl::getCurrentAction() {return currentAction;}

void StepperControl::attach(int p_pinIN1, int p_pinIN2, int p_pinIN3, int p_pinIN4) {
    if (!initialized) {
        Serial.println("Attaching stepper motor.");

        pinIN1 = p_pinIN1;
        pinIN2 = p_pinIN2;
        pinIN3 = p_pinIN3;
        pinIN4 = p_pinIN4;

        pinMode(pinIN1, OUTPUT);
        pinMode(pinIN2, OUTPUT);
        pinMode(pinIN3, OUTPUT);
        pinMode(pinIN4, OUTPUT);
    } else {
        Serial.println("Stepper motor already attached.");
    }
}

void StepperControl::processState() {
    if (initialized) {
        int nextPosition;

        switch (currentAction) {
            case StepperActions::Resting:
                if (actionChange) {
                    actionChange = false;
                }
                break;
            case StepperActions::CapStep:
                if (actionChange) {
                    Serial.println("Moving stepper motor one cap step.");
                    actionChange = false;
                }
                nextPosition = positionCounter + 1;
                if (nextPosition >= 12) { positionCounter = 0; }
                if (stepCounter != positionsSteps[positionCounter + 1]) {
                    rotateStep(RotationDirection::Clockwise);
                } else {
                    actionChange = true;
                    positionCounter = nextPosition;
                    currentAction = StepperActions::Stopping;
                }
                break;
            case StepperActions::Cycling:
                if (actionChange) {
                    Serial.println("Cycling stepper motor.");
                    actionChange = false;
                }
                nextPosition = positionCounter + 1;
                if (nextPosition >= 12) { positionCounter = 0; }
                if (stepCounter != positionsSteps[positionCounter + 1]) {
                    rotateStep(RotationDirection::Clockwise);
                } else {
                    positionCounter = nextPosition;
                }
                break;
            case StepperActions::Stopping:
                if (actionChange) {
                    Serial.println("Stopping stepper motor.");
                    actionChange = false;
                }
                break;
        }
    } else {
        Serial.println("Stepper motor not attached.");
    }
}

void StepperControl::doCycling() {
    if (initialized) {
        if (currentAction == StepperActions::Resting) {
            actionChange = true;
            currentAction = StepperActions::Cycling;
        }
    } else {
        Serial.println("Stepper motor not attached.");
    }
}

void StepperControl::doCapStep() {
    if (initialized) {
        if (currentAction == StepperActions::Resting) {
            actionChange = true;
            currentAction = StepperActions::CapStep;
        }
    } else {
        Serial.println("Stepper motor not attached.");
    }
}
void StepperControl::stopActions() {
    if (initialized) {
        if (currentAction == StepperActions::CapStep || currentAction == StepperActions::Cycling) {
            actionChange = true;
            currentAction = StepperActions::Stopping;
        }
    } else {
        Serial.println("Stepper motor not attached.");
    }
}

void StepperControl::rotateStep(enum RotationDirection direction) {
    switch (direction) {
        case RotationDirection::Clockwise:
            stepCounter++;
            if (stepCounter >= stepsTotal) stepCounter = 0;
            writeToStepper(stepCounter);
        case RotationDirection::CounterClockwise:
            stepCounter--;
            if (stepCounter < 0) stepCounter = stepsTotal - 1;
            writeToStepper(stepCounter);
    }
}

void StepperControl::writeToStepper(int p_step) {
    int step = stepsLookup[p_step];
    digitalWrite(pinIN1, bitRead(step, 0));
    digitalWrite(pinIN2, bitRead(step, 1));
    digitalWrite(pinIN3, bitRead(step, 2));
    digitalWrite(pinIN4, bitRead(step, 3));
}