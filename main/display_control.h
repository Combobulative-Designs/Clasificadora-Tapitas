#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
//#include "PString.h"

#include "common_stuff.h"

#ifndef CLASIFICADORA_TI3_DISPLAY_CONTROL_H
#define CLASIFICADORA_TI3_DISPLAY_CONTROL_H

class TextLine {
public:
    TextLine();
    void generateVisibleLine(int p_columns);

    char completeLine[30] = {0};
    char visibleLine[17] = {0};
    char oldCompleteLine[30] = {0};
    char oldVisibleLine[17] = {0};
    TextAlignment alignment;
};

class DisplayControl {
public:
    DisplayControl(byte p_address, int p_columns, int p_rows, int p_rest_delay);
    void initialize();
    void processState();

    void setLineText(char p_text[], int p_lineIndex, enum TextAlignment p_alignment);
    void navArrows();
    void noNavArrows();
    LiquidCrystal_I2C i2c_display;
    unsigned long getLastWrite();
    bool rested();

private:
    bool initialized;
    byte address;
    int rows;
    int columns;
    TextLine lines[2];
    int restDelay;
    unsigned long lastWrite;
    bool stateChanged;
    bool showNavArrows;
    bool backlightState;
};

#endif //CLASIFICADORA_TI3_DISPLAY_CONTROL_H
