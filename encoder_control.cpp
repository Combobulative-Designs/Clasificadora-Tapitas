#include <Arduino.h>

#include "common_stuff.h"

#include "encoder_control.h"

RotaryEncoderControl::RotaryEncoderControl() : initialized(false), pinCLK(0), pinDT(0), rotation(0), clkLastState(0) {}

int RotaryEncoderControl::getAngle() {return abs(rotation * 30);}

void RotaryEncoderControl::attach(int p_pin_clk, int p_pin_dt) {
    if (!initialized) {
        Serial.println("Attaching rotary encoder.");

        pinCLK = p_pin_clk;
        pinDT = p_pin_dt;

        pinMode(pinCLK, INPUT);
        pinMode(pinDT, INPUT);

        clkLastState = digitalRead(pinCLK);

        initialized = true;
    } else {
        Serial.println("Rotary encoder already attached.");
    }
}

void RotaryEncoderControl::resetRotation() {
    if (initialized) {
        rotation = 0;
    } else {
        Serial.println("Encoder not attached.");
    }
}

void RotaryEncoderControl::processState() {
    if (initialized) {
        int clkState = digitalRead(pinCLK);
        if (clkState != clkLastState) {
            if (digitalRead(pinDT) != clkState) {
                rotation ++;
            }
            else {
                rotation--;
            }
            if (rotation >= 30 ) {
                rotation = 0;
            }
        }
        clkLastState = clkState;
    } else {
        Serial.println("Encoder not attached.");
    }
}