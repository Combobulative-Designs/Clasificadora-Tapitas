#include <Arduino.h>
#include <Wire.h>

#include "common_stuff.h"

ButtonState::ButtonState() :
        pinButton(0),
        state(0),
        cyclesHeld(0),
        initialized(false),
        lastReading(millis())
{}

void ButtonState::attach(int p_pin_button, int p_debounce_delay) {
    if (!initialized) {
        Serial.println(F("Attaching button."));

        pinButton = p_pin_button;
        debounceDelay = p_debounce_delay;
        pinMode(pinButton, INPUT);

        initialized = true;
    } else {
        Serial.println(F("Button already attached."));
    }
}

void ButtonState::processState() {
    if (initialized) {
        lastReading = millis();
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
        Serial.println(F("Button not initialized."));
    }
}

int ButtonState::getState() {return state;}
int ButtonState::getCyclesHeld() {return cyclesHeld;}
unsigned long ButtonState::getLastReading() {return lastReading;}
ButtonAction ButtonState::getUserAction() {return userAction;}
bool ButtonState::debounced() {return (lastReading + debounceDelay <= millis());}

int I2CScanner(byte addrs[]) {
    Wire.begin();

    byte errCode, addr;
    int devCount;

    Serial.println(F("Starting I2C scan."));

    devCount = 0;
    for (addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        errCode = Wire.endTransmission();

        if (errCode == 0) {
            Serial.print(F("Found I2C device at 0x"));
            if (addr<16) {
                Serial.print(F("0"));
            }
            Serial.print(addr,HEX);
            Serial.println(".");

            addrs[devCount] = addr;
            devCount++;

        } else if (errCode == 4) {
            Serial.print(F("Weird error at 0x"));
            if (addr<16) {
                Serial.print(F("0"));
            }
            Serial.println(addr,HEX);
            Serial.println(F("."));
        }

    }
    if (devCount == 0) {
        Serial.println(F("No devices found."));
    } else {
        Serial.println(F("Scan complete."));
    }

    Wire.end();
    return devCount;
}

char* ConvertColorCategoryToChar(ColorCategory p_category) {
    switch (p_category) {
        case ColorCategory::Reds:
            return "Reds";
        case ColorCategory::Yellows:
            return "Yellows";
        case ColorCategory::Blues:
            return "Blues";
        case ColorCategory::Greens:
            return "Greens";
        case ColorCategory::Whites:
            return "Whites";
        case ColorCategory::Greys:
            return "Greys";
        default: //Blacks
            return "Blacks";
    }
}