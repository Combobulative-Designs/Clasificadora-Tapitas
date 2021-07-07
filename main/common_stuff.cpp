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

char* ConvertRGBColorToChar(RGBColor rgbColor) {
    char colorString[13];
    char colorChannelString[5];
    itoa((int)(rgbColor.red / rgbColor.white * 255.0), colorChannelString, 10);
    for (int i = 0; i < 3; i++) {
        colorString[i] = colorChannelString[i];
    }
    colorString[3] = ' ';
    colorString[4] = ' ';
    itoa((int)(rgbColor.blue / rgbColor.white * 255.0), colorChannelString, 10);
    for (int i = 0; i < 3; i++) {
        colorString[i+5] = colorChannelString[i];
    }
    colorString[8] = ' ';
    colorString[9] = ' ';
    itoa((int)(rgbColor.green / rgbColor.white * 255.0), colorChannelString, 10);
    for (int i = 0; i < 3; i++) {
        colorString[i+9] = colorChannelString[i];
    }
    colorString[12] = '!';
    colorString[13] = '\0';
    colorString[14] = '\0';
    colorString[15] = '\0';
    colorString[16] = '\0';
    colorString[17] = '\0';
    colorString[18] = '\0';
    colorString[19] = '\0';
    colorString[20] = '\0';
    return colorString;
};
