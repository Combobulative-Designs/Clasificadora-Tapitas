#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "PString.h"

#include "common_stuff.h"

#ifndef CLASIFICADORA_TI3_DISPLAY_CONTROL_H
#define CLASIFICADORA_TI3_DISPLAY_CONTROL_H

class TextLine {
public:
    TextLine();
    int getTotalCycles();
    void generateVisibleLine(int p_columns);

    char* completeLine = " ";
    char* visibleLine = " ";
    char* oldCompleteLine = " ";
    char* oldVisibleLine = " ";
    char lineBuffer[30];
    PString lineProcessor;
    int marqueeDelayBefore = 0;
    int marqueeDelayAfter = 0;
    int marqueeSpeed = 300;
    int marqueeCurrentIndex = 0;
    int marqueeCurrentCycle = 0;
    TextAlignment alignment;
};

class DisplayControl {
public:
    DisplayControl(byte p_address, int p_columns, int p_rows, int p_rest_delay);
    void initialize();
    void processState();

    void setLineText(char p_text[], int p_lineIndex, TextAlignment p_alignment);
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
};

#endif //CLASIFICADORA_TI3_DISPLAY_CONTROL_H