#include <Arduino.h>

#include "common_stuff.h"
#include "PROGMEM_readAnything.h"
#include "progmem_data.h"

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
        } else if (newState == LOW && state == HIGH) {
            userAction = ButtonAction::Release;
        } else if (newState == LOW && state == LOW) {
            userAction = ButtonAction::Rest;
        }

        state = newState;
    }
}

ButtonAction ButtonState::getUserAction() {return userAction;}
bool ButtonState::debounced() {return (lastReading + debounceDelay <= millis());}

enum ColorCategory RGBColorSampleIndexToCategory(int index) {
    switch (index) {
        case 0:
            return ColorCategory::Blacks;
        case 1:
            return ColorCategory::Whites;
        case 2:
            return ColorCategory::Reds;
        case 3:
            return ColorCategory::Yellows;
        case 4:
            return ColorCategory::Reds;
        case 5:
            return ColorCategory::Reds;
        case 6:
            return ColorCategory::Reds;
        case 7:
            return ColorCategory::Blues;
        case 8:
            return ColorCategory::Greens;
        case 9:
            return ColorCategory::Greens;
        case 10:
            return ColorCategory::Reds;
        case 11:
            return ColorCategory::Blues;
        default:
            return ColorCategory::Greys;
    }
}

/*
RGBColorNorm NormalizeRGBColor(RGBColor rgbColor) {
    RGBColorNorm rgbNorm;

    rgbNorm.red = (int)(((float)rgbColor.red / rgbColor.white) * 255.0);
    rgbNorm.green = (int)(((float)rgbColor.green / rgbColor.white) * 255.0);
    rgbNorm.blue = (int)(((float)rgbColor.blue / rgbColor.white) * 255.0);

    return rgbNorm;
}*/
/*
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

void ConvertRGBColorNormToChar(RGBColorNorm rgbNorm, char* rgbNormStr) {
    for (int i = 0; i < 12; i++) {
      rgbNormStr[i] = '\0';
    }
    
    char channelStr[5];
    itoa(rgbNorm.red, channelStr, 10);
    strcat(rgbNormStr, channelStr);
    strcat(rgbNormStr, (char *)",");
    itoa(rgbNorm.green, channelStr, 10);
    strcat(rgbNormStr, channelStr);
    strcat(rgbNormStr, (char *)",");
    itoa(rgbNorm.blue, channelStr, 10);
    strcat(rgbNormStr, channelStr);
}

void ConvertRGBSampleIndexToName(int sampleIndex, char* rgbName) {
    char rgbName11[11];
    PROGMEM_readAnything (&sampleNames [sampleIndex], rgbName11);
    rgbName = (char*)rgbName11;
};*/
