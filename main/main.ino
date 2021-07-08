#include <Servo.h>
#include <AccelStepper.h>

#include "common_stuff.h"


#include "sensor_control.h"
#include "servo_control.h"
#include "stepper_control.h"
#include "simple_output_control.h"
#include "rgb_control.h"
#include "display_control.h"
#include "menu_control.h"

#include "const_def.h"
#include "progmem_data.h"


ButtonState buttonPrev;
ButtonState buttonNext;
ButtonState buttonReturn;
ButtonState buttonEnter;

SensorControl sensorControl(PIN_SENSOR_AUX_LED);
StepperControl stepperControl(PIN_STEPPER_IN1, PIN_STEPPER_IN2, PIN_STEPPER_IN3, PIN_STEPPER_IN4);
ServoControl servoControl(PIN_SERVO_SIGNAL);
SOutputControl sOutputControl(PIN_RELAY_SIGNAL);
RGBControl rgbControl(PIN_RGB_LED_RED, PIN_RGB_LED_GRE, PIN_RGB_LED_BLU);
DisplayControl displayControl(DISPLAY_ADDRESS, DISPLAY_COLUMNS, DISPLAY_ROWS, REST_DELAY);
SorterControl sorterControl(sensorControl, stepperControl, servoControl, sOutputControl, rgbControl, displayControl);
MenuControl menuControl(menu2, displayControl, sorterControl);

void setup() {
    Serial.begin(9600);

    buttonPrev.attach(PIN_BUTTON_PREV, DEBOUNCE_DELAY);
    buttonNext.attach(PIN_BUTTON_NEXT, DEBOUNCE_DELAY);
    buttonReturn.attach(PIN_BUTTON_RETURN, DEBOUNCE_DELAY);
    buttonEnter.attach(PIN_BUTTON_ENTER, DEBOUNCE_DELAY);

    menuControl.initialize();
}

void loop() {
    sensorControl.processState();
    stepperControl.processState();
    sOutputControl.processState();
    rgbControl.processState();
    displayControl.processState();
    sorterControl.processState();
    menuControl.processState();

    buttonPrev.processState();
    buttonNext.processState();
    buttonReturn.processState();
    buttonEnter.processState();

    if (buttonPrev.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Previous);
    if (buttonNext.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);
    if (buttonReturn.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Return);
    if (buttonEnter.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Enter);
}
