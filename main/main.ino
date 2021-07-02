#include <Servo.h>
#include <AccelStepper.h>

#include "common_stuff.h"

#include "sensor_dummy.h"
#include "servo_control.h"
#include "stepper_control.h"
//#include "encoder_control.h"
#include "display_control.h"
#include "menu_control.h"
/*
#define PIN_BUTTON_PREV 2
#define PIN_BUTTON_NEXT 3
#define PIN_BUTTON_RETURN 4
#define PIN_BUTTON_ENTER 5
*//*
#define PIN_ENCODER_CLK 6
#define PIN_ENCODER_DT 7
*//*
#define PIN_STEPPER_IN1 8
#define PIN_STEPPER_IN2 9
#define PIN_STEPPER_IN3 10
#define PIN_STEPPER_IN4 11
*//*
#define PIN_SERVO_SIGNAL 12
*//*
#define PIN_SENSOR_LED 13
*//*
#define CYCLE_DURATION 20
#define DEBOUNCE_DELAY 50
#define REST_DELAY 50
*/
enum ColorCategory categoryDetected = ColorCategory::Blacks;

const MenuItemS PROGMEM menu_01 = {1, 0, 0, "TAPINATOR 9KPP\0\0\0", 17};
const MenuItemS PROGMEM menu_02 = {2, 1, 0, "Iniciar Ya\0\0\0\0\0\0\0\0\0", 0};
const MenuItemS PROGMEM menu_03 = {3, 1, 1, "Clasificar\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_04 = {4, 1, 2, "Configurar\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_05 = {5, 1, 3, "Estadistica\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_06 = {6, 3, 0, "Automatico\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_07 = {7, 6, 0, "Comenzar\0\0\0\0\0\0\0\0\0\0\0", 0};
const MenuItemS PROGMEM menu_08 = {8, 4, 0, "Calib. disco\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_09 = {9, 8, 0, "A reloj\0\0\0\0\0\0\0\0\0\0\0\0", 5};
const MenuItemS PROGMEM menu_10 = {10, 8, 1, "Contra reloj\0\0\0\0\0\0\0", 6};
const MenuItemS PROGMEM menu_11 = {11, 4, 1, "Acerca de\0\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_12 = {12, 11, 0, "Equipo\0\0\0\0\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_13 = {13, 12, 0, "Vogel J.\0\0\0\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_14 = {14, 12, 1, "Ponce.. E.\0\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_15 = {15, 12, 2, "Jorajuria S.\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_16 = {16, 12, 3, "Jauregui R.\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_17 = {17, 11, 1, "Clase\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_18 = {18, 17, 0, "U. Catolica\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_19 = {19, 17, 1, "2021 Sem 1\0\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_20 = {20, 17, 2, "TI3 Grupo B\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_21 = {21, 17, 3, "Equipo 12\0\0\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_22 = {22, 3, 1, "Manual\0\0\0\0\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_23 = {23, 22, 0, "Mover disco\0\0\0\0\0\0\0\0", 4};
const MenuItemS PROGMEM menu_24 = {24, 22, 1, "Leer color\0\0\0\0\0\0\0\0\0", 2};
const MenuItemS PROGMEM menu_25 = {25, 22, 2, "Mover sensor\0\0\0\0\0\0\0", 3};
const MenuItemS PROGMEM menu_26 = {26, 3, 2, "Paso a paso\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_27 = {27, 26, 0, "Siguiente\0\0\0\0\0\0\0\0\0\0", 1};
const MenuItemS PROGMEM menu_28 = {28, 5, 0, "Tiempo\0\0\0\0\0\0\0\0\0\0\0\0\0", 7};
const MenuItemS PROGMEM menu_29 = {29, 5, 1, "Cantidades\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_30 = {30, 29, 0, "Rojos\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 9};
const MenuItemS PROGMEM menu_31 = {31, 29, 1, "Azules\0\0\0\0\0\0\0\0\0\0\0\0\0", 10};
const MenuItemS PROGMEM menu_32 = {32, 29, 2, "Verdes\0\0\0\0\0\0\0\0\0\0\0\0\0", 11};
const MenuItemS PROGMEM menu_33 = {33, 29, 3, "Amarillos\0\0\0\0\0\0\0\0\0\0", 12};
const MenuItemS PROGMEM menu_34 = {34, 29, 4, "Blancos\0\0\0\0\0\0\0\0\0\0\0\0", 14};
const MenuItemS PROGMEM menu_35 = {35, 29, 5, "Grises\0\0\0\0\0\0\0\0\0\0\0\0\0", 15};
const MenuItemS PROGMEM menu_36 = {36, 29, 6, "Negros\0\0\0\0\0\0\0\0\0\0\0\0\0", 13};

const MenuItemS menu2[36] PROGMEM = {
  menu_01, menu_02, menu_03, menu_04, menu_05, menu_06, menu_07, menu_08, menu_09, menu_10,
  menu_11, menu_12, menu_13, menu_14, menu_15, menu_16, menu_17, menu_18, menu_19, menu_20,
  menu_21, menu_22, menu_23, menu_24, menu_25, menu_26, menu_27, menu_28, menu_29, menu_30,
  menu_31, menu_32, menu_33, menu_34, menu_35, menu_36
};

ButtonState buttonPrev;
ButtonState buttonNext;
ButtonState buttonReturn;
ButtonState buttonEnter;

StepperControl stepperControl(8, 9, 10, 11);
ServoControl servoControl(12);
DisplayControl displayControl(0x27, 16, 2, 50);
MenuControl menuControl(menu2, displayControl);


void setup() {
    Serial.begin(9600);

    buttonPrev.attach(2, 50);
    buttonNext.attach(3, 50);
    buttonReturn.attach(4, 50);
    buttonEnter.attach(5, 50);

    InitSensor();
    displayControl.initialize();
    stepperControl.initialize();
    servoControl.initialize();

    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);

    displayControl.noNavArrows();
    displayControl.setLineText(
            (char*)"TAPITANATOR 9KPP",
            0,
            TextAlignment::Center);
    displayControl.setLineText(
            (char*)"¡Bienvenido!",
            1,
            TextAlignment::Center);

    delay(3000);

    menuControl.initialize();
}

void loop() {
    buttonPrev.processState();
    buttonNext.processState();
    buttonReturn.processState();
    buttonEnter.processState();

    if (buttonPrev.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Previous);
    if (buttonNext.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Next);
    if (buttonReturn.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Return);
    if (buttonEnter.getUserAction() == ButtonAction::Release) menuControl.triggerUserAction(MenuUserActions::Enter);

    menuControl.processState();
    displayControl.processState();
}
