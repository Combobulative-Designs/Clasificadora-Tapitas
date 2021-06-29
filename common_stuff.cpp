#include <Arduino.h>
#include <Wire.h>

#include "common_stuff.h"

ButtonState::ButtonState() : pinButton(0), state(0), cyclesHeld(0), initialized(false) {}

void ButtonState::attach(int p_pin_button) {
    if (!initialized) {
        Serial.println("Attaching button.");

        pinButton = p_pin_button;
        pinMode(pinButton, INPUT);

        initialized = true;
    } else {
        Serial.println("Button already attached.");
    }
}

void ButtonState::processState() {
    if (initialized) {
        int newState = digitalRead(pinButton);

        if (newState == HIGH && state == LOW) {
            userAction = ButtonAction::Press;
        } else if (newState == HIGH && state == HIGH) {
            userAction = ButtonAction::Hold;
            cyclesHeld += 1;
        } else if (newState == LOW && state == HIGH) {
            userAction = ButtonAction::Release;
            cyclesHeld = 0;
        } else if (newState == LOW && state == LOW) {
            userAction = ButtonAction::Rest;
        }

        state = newState;
    } else {
        Serial.println("Button not initialized.");
    }
}

int ButtonState::getState() {return state;}
int ButtonState::getCyclesHeld() {return cyclesHeld;}
ButtonAction ButtonState::getUserAction() {return userAction;}

void I2CScanner() {
    Wire.begin();

    byte errCode, addr;
    int devCount;

    Serial.println("Starting I2C scan.");

    devCount = 0;
    for (addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        errCode = Wire.endTransmission();

        if (errCode == 0) {
            Serial.print("Found I2C device at 0x");
            if (addr<16) {
                Serial.print("0");
            }
            Serial.print(addr,HEX);
            Serial.println(".");

            devCount++;

        } else if (errCode == 4) {
            Serial.print("Weird error at 0x");
            if (addr<16) {
                Serial.print("0");
            }
            Serial.println(addr,HEX);
            Serial.println(".");
        }

    }
    if (devCount == 0) {
        Serial.println("No devices found.");
    } else {
        Serial.println("Scan complete.");
    }

    Wire.end();
}

char* ConvertColorCategoryToChar(ColorCategory p_category) {
    switch (p_category) {
        case ColorCategory::Reds:
            return (char*)"Reds";
        case ColorCategory::Yellows:
            return (char*)"Yellows";
        case ColorCategory::Blues:
            return (char*)"Blues";
        case ColorCategory::Greens:
            return (char*)"Greens";
        case ColorCategory::Whites:
            return (char*)"Whites";
        case ColorCategory::Greys:
            return (char*)"Greys";
        default: //Blacks
            return (char*)"Blacks";
    }
}

char* RepeatChar(char p_char, int p_repetitions) {
    char* resutlingString;

    for (int i = 0; i < p_repetitions; i++) {
        resutlingString += p_char;
    }

    return resutlingString;
}