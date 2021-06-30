#include <Servo.h>
#include <AccelStepper.h>
#include <LiquidCrystal_I2C.h>

#include "common_stuff.h"

#include "sensor_dummy.h"
#include "servo_control.h"
#include "stepper_control.h"
#include "encoder_control.h"
#include "display_control.h"

#define PIN_STEPPER_BUTTON 2
#define PIN_SERVO_BUTTON 3
#define PIN_SENSOR_BUTTON 4
#define PIN_PANIC_BUTTON 5

#define PIN_ENCODER_CLK 6
#define PIN_ENCODER_DT 7

#define PIN_STEPPER_IN1 8
#define PIN_STEPPER_IN2 9
#define PIN_STEPPER_IN3 10
#define PIN_STEPPER_IN4 11

#define PIN_SERVO_SIGNAL 12

#define PIN_SENSOR_LED 13

#define CYCLE_DURATION 20

enum ColorCategory categoryDetected = ColorCategory::Blacks;

ButtonState sensorButton;
ButtonState servoButton;
ButtonState stepperButton;
ButtonState panicButton;

RotaryEncoderControl encoderControl;
StepperControl stepperControl(PIN_STEPPER_IN1, PIN_STEPPER_IN2, PIN_STEPPER_IN3, PIN_STEPPER_IN4);
ServoControl servoControl(PIN_SERVO_SIGNAL);
DisplayControl displayControl(0x27, 16, 2);


void setup() {
    Serial.begin(9600);

    sensorButton.attach(PIN_SENSOR_BUTTON);
    servoButton.attach(PIN_SERVO_BUTTON);
    stepperButton.attach(PIN_STEPPER_BUTTON);
    panicButton.attach(PIN_PANIC_BUTTON);

    InitSensor();
    displayControl.initialize();
    stepperControl.initialize();
    servoControl.initialize();
    /*encoderControl.attach(PIN_ENCODER_CLK,
                          PIN_ENCODER_DT);*/
    
    pinMode(PIN_SENSOR_LED, OUTPUT);
    digitalWrite(PIN_SENSOR_LED, HIGH);
}

void loop() {
    ProcessServoButton();
    ProcessStepperButton();
    ProcessSensorButton();
    ProcessPanicButton();
    ProcessDisplayState();
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
        if (stepperButton.getSecondsHeld() >= 3 and stepperControl.getCurrentAction() == StepperActions::Resting) {
            stepperControl.doCycling();
            //encoderControl.resetRotation();
        }
    } else if (stepperButton.getUserAction() == ButtonAction::Release) {
        if (stepperControl.getCurrentAction() == StepperActions::Resting) {
            stepperControl.doCapStep();
            //encoderControl.resetRotation();
        } else if (stepperControl.getCurrentAction() == StepperActions::Cycling) {
            stepperControl.stopActions();
        }
    }

    stepperControl.processState();
}

void ProcessSensorButton() {
    sensorButton.processState();

    if (sensorButton.getUserAction() == ButtonAction::Release) {
        categoryDetected = ReadColor();
        displayControl.setLineText(ConvertColorCategoryToChar(categoryDetected), 1, TextAlignment::Right);
    }
}

void ProcessPanicButton() {
    panicButton.processState();

    if (panicButton.getUserAction() == ButtonAction::Release) {
        stepperControl.stopActions();
    }
}

void ProcessDisplayState() {
    displayControl.processState();
}
