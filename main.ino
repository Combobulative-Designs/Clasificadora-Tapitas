#include <Servo.h>
#include <AccelStepper.h>

#include "common_stuff.h"

#include "sensor_dummy.h"
//#include "servo_control.h"
//#include "stepper_control.h"
//#include "encoder_control.h"
#include "display_control.h"
#include "menu_control.h"

/*#define PIN_STEPPER_BUTTON 2
#define PIN_SERVO_BUTTON 3
#define PIN_SENSOR_BUTTON 4
#define PIN_PANIC_BUTTON 5*/

#define PIN_BUTTON_PREV 2
#define PIN_BUTTON_NEXT 3
#define PIN_BUTTON_RETURN 4
#define PIN_BUTTON_ENTER 5

#define PIN_ENCODER_CLK 6
#define PIN_ENCODER_DT 7

#define PIN_STEPPER_IN1 8
#define PIN_STEPPER_IN2 9
#define PIN_STEPPER_IN3 10
#define PIN_STEPPER_IN4 11

#define PIN_SERVO_SIGNAL 12

#define PIN_SENSOR_LED 13

#define CYCLE_DURATION 20
#define DEBOUNCE_DELAY 50
#define REST_DELAY 50

enum ColorCategory categoryDetected = ColorCategory::Blacks;

const MenuItem menu[36] = {
        MenuItem(1, 0, 0, "TAPITANATOR 9KPP", MenuActions::None),
        MenuItem(2, 1, 0, "Iniciar Ya", MenuActions::RunAuto),
        MenuItem(3, 1, 1, "Clasificar", MenuActions::NavigateToId),
        MenuItem(4, 1, 2, "Configurar", MenuActions::NavigateToId),
        MenuItem(5, 1, 3, "Estadistica", MenuActions::NavigateToId),
        MenuItem(6, 3, 0, "Automatico", MenuActions::NavigateToId),
        MenuItem(7, 6, 0, "Comenzar", MenuActions::RunAuto),
        MenuItem(8, 4, 0, "Calib. disco", MenuActions::NavigateToId),
        MenuItem(9, 8, 0, "A reloj", MenuActions::DoStepperCWCycling),
        MenuItem(10, 8, 1, "Contra reloj", MenuActions::DoStepperCCWCycling),
        MenuItem(11, 4, 0, "Acerca de", MenuActions::NavigateToId),
        MenuItem(12, 11, 0, "Equipo", MenuActions::NavigateToId),
        MenuItem(13, 12, 0, "Vogel J.", MenuActions::None),
        MenuItem(14, 12, 1, "Ponce.. E.", MenuActions::None),
        MenuItem(15, 12, 2, "Jorajuria S.", MenuActions::None),
        MenuItem(16, 12, 3, "Jauregui R.", MenuActions::None),
        MenuItem(17, 11, 1, "Clase", MenuActions::NavigateToId),
        MenuItem(18, 17, 0, "U. Catolica", MenuActions::None),
        MenuItem(19, 17, 1, "2021 Sem 1", MenuActions::None),
        MenuItem(20, 17, 2, "TI3 Grupo B", MenuActions::None),
        MenuItem(21, 17, 3, "Equipo 12", MenuActions::None),
        MenuItem(22, 3, 1, "Manual", MenuActions::NavigateToId),
        MenuItem(23, 22, 0, "Mover disco", MenuActions::DoStepperCapStep),
        MenuItem(24, 22, 1, "Leer color", MenuActions::DoSensorReading),
        MenuItem(25, 22, 2, "Mover sensor", MenuActions::DoServoTurn),
        MenuItem(26, 3, 2, "Paso a paso", MenuActions::NavigateToId),
        MenuItem(27, 26, 0, "Siguiente", MenuActions::RunStep),
        MenuItem(28, 5, 0, "Tiempo", MenuActions::ShowTime),
        MenuItem(29, 5, 1, "Cantidades", MenuActions::NavigateToId),
        MenuItem(30, 29, 0, "Rojos", MenuActions::ShowAmountReds),
        MenuItem(31, 29, 1, "Azules", MenuActions::ShowAmountBlues),
        MenuItem(32, 29, 2, "Verdes", MenuActions::ShowAmountGreens),
        MenuItem(33, 29, 3, "Amarillos", MenuActions::ShowAmountYellows),
        MenuItem(34, 29, 4, "Blancos", MenuActions::ShowAmountWhites),
        MenuItem(35, 29, 5, "Grises", MenuActions::ShowAmountGreys),
        MenuItem(36, 29, 6, "Negros", MenuActions::ShowAmountBlacks)


};

// Codigo para pruebas con operacion por boton:
/*ButtonState sensorButton;
ButtonState servoButton;
ButtonState stepperButton;
ButtonState panicButton;*/


// Codigo para operacion por menu
ButtonState buttonPrev;
ButtonState buttonNext;
ButtonState buttonReturn;
ButtonState buttonEnter;

//RotaryEncoderControl encoderControl;
//StepperControl stepperControl(PIN_STEPPER_IN1, PIN_STEPPER_IN2, PIN_STEPPER_IN3, PIN_STEPPER_IN4);
//ServoControl servoControl(PIN_SERVO_SIGNAL);
DisplayControl displayControl(0x27, 16, 2, REST_DELAY);
MenuControl menuControl(menu, displayControl);


void setup() {
    Serial.begin(9600);

    /*sensorButton.attach(PIN_SENSOR_BUTTON, DEBOUNCE_DELAY);
    servoButton.attach(PIN_SERVO_BUTTON, DEBOUNCE_DELAY);
    stepperButton.attach(PIN_STEPPER_BUTTON, DEBOUNCE_DELAY);
    panicButton.attach(PIN_PANIC_BUTTON, DEBOUNCE_DELAY);*/

    buttonPrev.attach(PIN_BUTTON_PREV, DEBOUNCE_DELAY);
    buttonNext.attach(PIN_BUTTON_NEXT, DEBOUNCE_DELAY);
    buttonReturn.attach(PIN_BUTTON_RETURN, DEBOUNCE_DELAY);
    buttonEnter.attach(PIN_BUTTON_ENTER, DEBOUNCE_DELAY);

    //InitSensor();
    displayControl.initialize();
    //stepperControl.initialize();
    //servoControl.initialize();
    /*encoderControl.attach(PIN_ENCODER_CLK,
                          PIN_ENCODER_DT);*/

    pinMode(PIN_SENSOR_LED, OUTPUT);
    digitalWrite(PIN_SENSOR_LED, HIGH);

    displayControl.noNavArrows();
    displayControl.setLineText(
            "TAPITANATOR 9KPP",
            0,
            TextAlignment::Center);
    displayControl.setLineText(
            "¡Bienvenido!",
            1,
            TextAlignment::Center);

    delay(5000);

    menuControl.initialize();
}

void loop() {
    /*ProcessServoButton();
    ProcessStepperButton();
    ProcessSensorButton();
    ProcessPanicButton();*/

    /*ProcessButtonsState();
    ProcessMenuState();
    ProcessDisplayState();*/


    buttonPrev.processState();
    buttonNext.processState();
    buttonReturn.processState();
    buttonEnter.processState();

    if (buttonPrev.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);
    if (buttonNext.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);
    if (buttonReturn.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);
    if (buttonEnter.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);


    menuControl.processState();

    displayControl.processState();
}

/*void ProcessButtonsState() {
    buttonPrev.processState();
    buttonNext.processState();
    buttonReturn.processState();
    buttonEnter.processState();

    if (buttonPrev.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);
    if (buttonNext.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);
    if (buttonReturn.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);
    if (buttonEnter.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);
}

void ProcessMenuState() {
    menuControl.processState();
}

void ProcessDisplayState() {
    if (displayControl.rested()) {
        displayControl.processState();
    }
}*/









/*
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
        displayControl.setLineText("TAPITANATOR 9KPP", 0, TextAlignment::Left);
        displayControl.setLineText(ConvertColorCategoryToChar(categoryDetected), 1, TextAlignment::Center);
    }
}

void ProcessPanicButton() {
    panicButton.processState();

    if (panicButton.getUserAction() == ButtonAction::Release) {
        stepperControl.stopActions();
    }
}

void ProcessDisplayState() {
    if (displayControl.rested()) {
        displayControl.processState();
    }
}*/
