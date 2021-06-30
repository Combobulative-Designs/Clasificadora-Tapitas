#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "common_stuff.h"

#ifndef CLASIFICADORA_TI3_DISPLAY_CONTROL_H
#define CLASIFICADORA_TI3_DISPLAY_CONTROL_H

class TextLine {
public:
    TextLine();
    int getTotalCycles();

    char* completeLine;
    char* visibleLine;
    char* oldCompleteLine;
    int marqueeDelayBefore = 0;
    int marqueeDelayAfter = 0;
    int marqueeSpeed = 300;
    int marqueeCurrentIndex = 0;
    int marqueeCurrentCycle = 0;
    TextAlignment alignment;
};

class DisplayControl {
public:
    DisplayControl(byte address, int columns, int rows);
    void initialize();
    void processState();

    void setLineText(char* p_text, int p_lineIndex, TextAlignment p_alignment);
    LiquidCrystal_I2C i2c_display;

private:
    bool initialized;
    byte address;
    int rows;
    int columns;
    TextLine lines[2];
};

#endif //CLASIFICADORA_TI3_DISPLAY_CONTROL_H
