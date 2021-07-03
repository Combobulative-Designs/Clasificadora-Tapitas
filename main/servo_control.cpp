#include <Arduino.h>
#include <Servo.h>

#include "common_stuff.h"

#include "servo_control.h"

ServoControl::ServoControl(int p_pin_servo_signal) :
    rotation_black(0),
    initialized(false),
    pinSignal(p_pin_servo_signal)
    {}

void ServoControl::initialize() {
    if (!initialized) {
        servo_obj.attach(pinSignal, 500, 2500);
        servo_obj.write(rotation_black);
        initialized = true;
    }
}

void ServoControl::moveToColor(ColorCategory p_category) {
    if (initialized) {
        int rotation;
        switch (p_category) {
            case ColorCategory::Reds:
                rotation = rotation_black + 180;
                break;
            case ColorCategory::Yellows:
                rotation = rotation_black + 150;
                break;
            case ColorCategory::Blues:
                rotation = rotation_black + 120;
                break;
            case ColorCategory::Greens:
                rotation = rotation_black + 90;
                break;
            case ColorCategory::Whites:
                rotation = rotation_black + 60;
                break;
            case ColorCategory::Greys:
                rotation = rotation_black + 30;
                break;
            default: //Blacks
                rotation = rotation_black;
        }
        servo_obj.write(rotation);
    }
}
