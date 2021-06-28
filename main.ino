#include <Servo.h>

#include "common_stuff.h"

#include "sensor_dummy.h"
#include "servo_control.h"
#include "stepper_control.h"
#include "encoder_control.h"

#define PIN_SERVO_BUTTON 2
#define PIN_PANIC_BUTTON 3
#define PIN_STEPPER_BUTTON 4
#define PIN_SENSOR_BUTTON 5

#define PIN_ENCODER_CLK 6
#define PIN_ENCODER_DT 7

#define PIN_STEPPER_IN1 8
#define PIN_STEPPER_IN2 9
#define PIN_STEPPER_IN3 10
#define PIN_STEPPER_IN4 11

#define PIN_SERVO_SIGNAL 12

#define CYCLE_DURATION 20

enum ColorCategory categoryDetected = ColorCategory::Blacks;

ButtonState sensorButton;
ButtonState servoButton;
ButtonState stepperButton;
ButtonState panicButton;

RotaryEncoderControl encoderControl;
StepperControl stepperControl;
ServoControl servoControl;

void setup() {
    Serial.begin(9600);

    sensorButton.attach(PIN_SENSOR_BUTTON);
    servoButton.attach(PIN_SERVO_BUTTON);
    stepperButton.attach(PIN_STEPPER_BUTTON);
    panicButton.attach(PIN_PANIC_BUTTON);

    InitSensor();
    encoderControl.attach(PIN_ENCODER_CLK,
                          PIN_ENCODER_DT);
    stepperControl.attach(PIN_STEPPER_IN1,
                   PIN_STEPPER_IN2,
                   PIN_STEPPER_IN3,
                   PIN_STEPPER_IN4);
    servoControl.attach(PIN_SERVO_SIGNAL);
}

void loop() {
    ProcessServoButton();
    ProcessStepperButton();
    ProcessSensorButton();
    ProcessPanicButton();

    delay(CYCLE_DURATION);
}


void ProcessServoButton() {
    servoButton.processState();

    if (servoButton.getUserAction() == ButtonAction::Release) {
        servoControl.moveToColor(categoryDetected);
    }
}

void ProcessStepperButton() {
    stepperButton.processState();

    if (stepperButton.getUserAction() == ButtonAction::Hold) {
        if (stepperButton.getCyclesHeld() >= 150 and stepperControl.getCurrentAction() == StepperActions::Resting) {
            stepperControl.doCycling();
            encoderControl.resetRotation();
        }
    } else if (stepperButton.getUserAction() == ButtonAction::Release) {
        if (stepperControl.getCurrentAction() == StepperActions::Resting) {
            stepperControl.doCapStep();
            encoderControl.resetRotation();
        } else if (stepperControl.getCurrentAction() == StepperActions::Cycling) {
            stepperControl.stopActions();
        }
    }

    if (stepperControl.getCurrentAction() == StepperActions::Stopping) {
        char* angleStr;
        sprintf(angleStr, "%s%d%s", "Stepper moved ", encoderControl.getAngle(), " degrees.");
        Serial.println(angleStr);
    }

    stepperControl.processState();
}

void ProcessSensorButton() {
    sensorButton.processState();

    if (sensorButton.getUserAction() == ButtonAction::Release) {
        categoryDetected = ReadColorDummy();
        //categoryDetected = ReadColor();
    }
}

void ProcessPanicButton() {
    panicButton.processState();

    if (panicButton.getUserAction() == ButtonAction::Release) {
        stepperControl.stopActions();
        Serial.println("PANIK!!!");
    }
}
