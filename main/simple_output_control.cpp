#include <Arduino.h>

#include "simple_output_control.h"

SOutputControl::SOutputControl(int p_pin_signal) :
    pinSignal(p_pin_signal),
    stateChanged(false),
    currentState(LOW),
    initialized(false)
    {}

void SOutputControl::initialize() {
    if (!initialized) {
        pinMode(pinSignal, OUTPUT);
        initialized = true;
    }
}

void SOutputControl::on() {
    if (initialized) {
        currentState = HIGH;
        stateChanged = true;
    }
}


void SOutputControl::off() {
    if (initialized) {
        currentState = LOW;
        stateChanged = true;
    }
}

void SOutputControl::processState() {
    if (initialized and stateChanged) {
        stateChanged = false;
        digitalWrite(pinSignal, currentState);
    }
}
