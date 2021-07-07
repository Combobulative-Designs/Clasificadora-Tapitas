#include <Arduino.h>

#include "common_stuff.h"

ButtonState::ButtonState() :
        pinButton(0),
        state(0),
        initialized(false),
        lastReading(millis())
{}

void ButtonState::attach(int p_pin_button, int p_debounce_delay) {
    if (!initialized) {

        pinButton = p_pin_button;
        debounceDelay = p_debounce_delay;
        pinMode(pinButton, INPUT);

        initialized = true;
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
            //cyclesHeld += 1;
        } else if (newState == LOW && state == HIGH) {
            userAction = ButtonAction::Release;
            //cyclesHeld = 0;
        } else if (newState == LOW && state == LOW) {
            userAction = ButtonAction::Rest;
        }

        state = newState;
    }
}

ButtonAction ButtonState::getUserAction() {return userAction;}
bool ButtonState::debounced() {return (lastReading + debounceDelay <= millis());}

RGBColorNorm NormalizeRGBColor(RGBColor rgbColor) {
    RGBColorNorm rgbNorm;

    rgbNorm.red = (int)(((float)rgbColor.red / rgbColor.white) * 255.0);
    rgbNorm.blue = (int)(((float)rgbColor.blue / rgbColor.white) * 255.0);
    rgbNorm.green = (int)(((float)rgbColor.green / rgbColor.white) * 255.0);

    return rgbNorm;
}

char* ConvertColorCategoryToChar(ColorCategory p_category) {
    switch (p_category) {
        case ColorCategory::Reds:
            return (char*)F("Reds");
        case ColorCategory::Yellows:
            return (char*)F("Yellows");
        case ColorCategory::Blues:
            return (char*)F("Blues");
        case ColorCategory::Greens:
            return (char*)F("Greens");
        case ColorCategory::Whites:
            return (char*)F("Whites");
        case ColorCategory::Greys:
            return (char*)F("Greys");
        default: //Blacks
            return (char*)F("Blacks");
    }
}

char* ConvertRGBColorToChar(RGBColor rgbColor, char* rgbNormStr) { 
    RGBColorNorm rgbNorm = NormalizeRGBColor(rgbColor);
    
    for (int i = 0; i < 20; i++) {
      rgbNormStr[i] = '\0';
    }
    char channelStr[3];
    itoa(rgbNorm.red, channelStr, 10);
    for (int i = 0; i < (log10(rgbNorm.red) + 1); i++) {
        rgbNormStr[i] = channelStr[i];
    }
    rgbNormStr[strlen(rgbNormStr)] = ',';
    itoa(rgbNorm.blue, channelStr, 10);
    for (int i = 0; i < (log10(rgbNorm.blue) + 1); i++) {
        rgbNormStr[i+strlen(rgbNormStr)] = channelStr[i];
    }
    rgbNormStr[strlen(rgbNormStr)] = ',';
    itoa(rgbNorm.green, channelStr, 10);
    for (int i = 0; i < (log10(rgbNorm.green) + 1); i++) {
        rgbNormStr[i+strlen(rgbNormStr)] = channelStr[i];
    }
};
