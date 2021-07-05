#include <Arduino.h>

#include "common_stuff.h"

#include "rgb_control.h"

int colors[9][3] = {
        {000, 000, 000}, //Off
        {255, 000, 000}, //Red
        {000, 255, 000}, //Green
        {000, 000, 255}, //Blue
        {000, 255, 255}, //Cyan
        {255, 000, 255}, //Magenta
        {255, 255, 000}, //Yellow
        {255, 255, 255}, //White
        {255, 255, 125}  //Raspberry
};

RGBControl::RGBControl(int p_pin_red, int p_pin_green, int p_pin_blue) :
    pinRed(p_pin_red),
    pinGreen(p_pin_green),
    pinBlue(p_pin_blue),
    initialized(false),
    stateChanged(false)
    {}

void RGBControl::initialize() {
    if (!initialized) {
        color = RGBColors::Off;
        stateChanged = true;
        initialized = true;
        lastActivity = millis();
    }
}

void RGBControl::setColor(enum RGBColors p_color) {
    if (initialized and isRested()) {
        color = p_color;
        stateChanged = true;
        lastActivity = millis();
    }
}

bool RGBControl::isRested() {
    return (millis() - lastActivity >= 500);
}

void RGBControl::processState() {
    if (initialized) {
        if (stateChanged) {
            lastActivity = millis();
            stateChanged = false;
            writeToLED(
                    colors[(int)color][0],
                    colors[(int)color][1],
                    colors[(int)color][2]
                    );
        }
    }
}

void RGBControl::writeToLED(int p_value_red, int p_value_green, int p_value_blue) {
    analogWrite(pinRed, p_value_red);
    analogWrite(pinGreen, p_value_green);
    analogWrite(pinBlue, p_value_blue);
}
