#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "PString.h"

#include "common_stuff.h"

#include "display_control.h"

byte arrow_right[8] = {
        B10000,
        B11000,
        B11100,
        B11110,
        B11100,
        B11000,
        B10000,
        B00000
};
byte arrow_left[8] = {
        B00010,
        B00110,
        B01110,
        B11110,
        B01110,
        B00110,
        B00010,
        B00000
};
byte arrow_return[8] = {
        B00000,
        B01000,
        B11100,
        B01000,
        B01000,
        B01110,
        B00000,
        B00000
};


DisplayControl::DisplayControl(byte p_address, int p_columns, int p_rows, int p_rest_delay) :
        initialized(false),
        i2c_display(p_address, 16, 2),
        rows(2),
        columns(16),
        lines({TextLine(), TextLine()}),
        lastWrite(millis()),
        restDelay(p_rest_delay),
        stateChanged(false),
        backlightState(false)
{}
TextLine::TextLine() :
        alignment(TextAlignment::Left),
        lineProcessor(lineBuffer, sizeof(lineBuffer))
{}

void DisplayControl::initialize() {
    if (!initialized) {
        Serial.println(F("Initializing display.."));
        i2c_display.init();
        i2c_display.backlight();
        i2c_display.print(F("Initializing..."));
        i2c_display.noCursor();
        backlightState = true;
        initialized = true;
        i2c_display.createChar(1, arrow_left);
        i2c_display.createChar(2, arrow_right);
        i2c_display.createChar(3, arrow_return);
    } else {
        Serial.println(F("Display already initialized."));
    }
}

int TextLine::getTotalCycles() {
    if (strlen(completeLine) > 16) {
        return marqueeDelayAfter + marqueeDelayBefore + ((strlen(completeLine) - 15) * marqueeSpeed);
    } else {
        return 0;
    }
}

void TextLine::generateVisibleLine(int p_columns) {
    lineProcessor.begin();
    if (strlen(completeLine) == p_columns) {
        lineProcessor.print(completeLine);
    } else if (strlen(completeLine) < p_columns) {
        if (alignment == TextAlignment::Left) {
            lineProcessor.print(completeLine);
            while (lineProcessor.length() < 16) lineProcessor.print(" ");
        }
        if (alignment == TextAlignment::Right) {
            while (lineProcessor.length() < (16 - strlen(completeLine))) lineProcessor.print(" ");
            lineProcessor.print(completeLine);
        }
        if (alignment == TextAlignment::Center) {
            while (lineProcessor.length() < (int)((16 - strlen(completeLine)) / 2)) lineProcessor.print(" ");
            lineProcessor.print(completeLine);
            while (lineProcessor.length() < 16) lineProcessor.print(" ");
        }
    }

    for (int i = 0; i < 16; i++) {
        visibleLine[i] = lineProcessor[i];
    }
    visibleLine[16] = '\0';
}

void DisplayControl::processState() {
    if (backlightState and rested()) {i2c_display.noBacklight(); backlightState = false; }
    else if ((!backlightState) and (!rested())) {i2c_display.backlight(); backlightState = true; }

    for (int row=0; row<rows; ++row) {
        if (lines[row].completeLine != lines[row].oldCompleteLine) {
            for (int i = 0; i < 30; i++) lines[row].oldCompleteLine[i] = lines[row].completeLine[i];
            lines[row].generateVisibleLine(columns);
            for (int i = 0; i < 17; i++) lines[row].oldVisibleLine[i] = lines[row].visibleLine[i];
            stateChanged = true;
        }
    }

    if (stateChanged) {
        stateChanged = false;
        lastWrite = millis();
        i2c_display.setCursor(0, 0);
        i2c_display.print(lines[0].visibleLine);
        i2c_display.setCursor(0, 1);
        i2c_display.print(lines[1].visibleLine);

        if (showNavArrows) {
            i2c_display.setCursor(0, 1);
            i2c_display.write(byte(1));
            i2c_display.setCursor(15, 1);
            i2c_display.write(byte(0));
        }
    }
}

void DisplayControl::setLineText(char p_text[], int p_lineIndex, enum TextAlignment p_alignment) {
    for (int i = 0; i < strlen(p_text); i++) lines[p_lineIndex].completeLine[i] = p_text[i];
    lines[p_lineIndex].completeLine[strlen(p_text)] = '\0' ;
    for (int i = strlen(lines[p_lineIndex].completeLine) + 1; i < 30; i++) lines[p_lineIndex].completeLine[i] = 0;
    lines[p_lineIndex].alignment = p_alignment;
}

void DisplayControl::navArrows() { showNavArrows = true; }
void DisplayControl::noNavArrows() { showNavArrows = false; }

bool DisplayControl::rested() {return (lastWrite + restDelay <= millis());}
