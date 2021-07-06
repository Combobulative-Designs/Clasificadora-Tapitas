#include <Arduino.h>

#include "relay_control.h"

RelayControl::RelayControl(int p_pin_signal) :
    pinSignal(p_pin_signal),
    stateChanged(false),
    currentState(LOW),
    initialized(false)
    {}

void RelayControl::initialize() {
    if (!initialized) {
        pinMode(pinSignal, OUTPUT);
        initialized = true;
    }
}

void RelayControl::on() {
    if (initialized) {
        currentState = HIGH;
        stateChanged = true;
    }
}


void RelayControl::off() {
    if (initialized) {
        currentState = LOW;
        stateChanged = true;
    }
}

void RelayControl::processState() {
    if (initialized and stateChanged) {
        stateChanged = false;
        digitalWrite(pinSignal, currentState);
    }
}
