#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
//#include "PString.h"

#include "common_stuff.h"

#include "display_control.h"

const byte arrow_right[8] = {
        B10000,
        B11000,
        B11100,
        B11110,
        B11100,
        B11000,
        B10000,
        B00000
};
const byte arrow_left[8] = {
        B00010,
        B00110,
        B01110,
        B11110,
        B01110,
        B00110,
        B00010,
        B00000
};
const byte arrow_return[8] = {
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
        alignment(TextAlignment::Left)
{}

void DisplayControl::initialize() {
    if (!initialized) {
        i2c_display.init();
        i2c_display.backlight();
        i2c_display.print(F("Initializing..."));
        i2c_display.noCursor();
        backlightState = true;
        initialized = true;
        i2c_display.createChar(1, (uint8_t*)arrow_left);
        i2c_display.createChar(2, (uint8_t*)arrow_right);
        i2c_display.createChar(3, (uint8_t*)arrow_return);
    }
}

void TextLine::generateVisibleLine(int p_columns) {
    if (strlen(completeLine) == p_columns) {
        for (int i = 0; i < strlen(completeLine); i++) visibleLine[i] = completeLine[i];
        visibleLine[16] = '\0';
    } else if (strlen(completeLine) < p_columns) {
        if (alignment == TextAlignment::Left) {
            for (int i = 0; i < strlen(completeLine); i++) visibleLine[i] = completeLine[i];
            for (int i = strlen(completeLine); i < 16; i++) visibleLine[i] = ' ';
            visibleLine[16] = '\0';
        }
        if (alignment == TextAlignment::Right) {
            for (int i = 0; i < (16 - strlen(completeLine)); i++) visibleLine[i] = ' ';
            for (int i = 0; i < strlen(completeLine); i++) visibleLine[i + strlen(completeLine)] = completeLine[i];
            visibleLine[16] = '\0';
        }
        if (alignment == TextAlignment::Center) {
            for (int i = 0; i < (int)((16 - strlen(completeLine)) / 2); i++) visibleLine[i] = ' ';
            for (int i = 0; i < strlen(completeLine); i++) visibleLine[i + (int)((16 - strlen(completeLine)) / 2)] = completeLine[i];
            for (int i = 0; i < ((int)((16 - strlen(completeLine)) / 2) + strlen(completeLine)); i++) visibleLine[i + ((int)((16 - strlen(completeLine)) / 2) + strlen(completeLine))] = ' ';
            visibleLine[16] = '\0';
        }
    }
}

void DisplayControl::processState() {
    /*if (backlightState and rested()) {i2c_display.noBacklight(); backlightState = false; }
    else if ((!backlightState) and (!rested())) {i2c_display.backlight(); backlightState = true; }*/

    for (int row=0; row<rows; ++row) {
        if (stateChanged) {
            for (int i = 0; i < 30; i++) lines[row].oldCompleteLine[i] = lines[row].completeLine[i];
            lines[row].generateVisibleLine(columns);
            for (int i = 0; i < 17; i++) lines[row].oldVisibleLine[i] = lines[row].visibleLine[i];
            //stateChanged = true;
        }
    }

    if (stateChanged) {
        lastWrite = millis();
        i2c_display.setCursor(0, 0);
        i2c_display.print(lines[0].visibleLine);
        i2c_display.setCursor(0, 1);
        i2c_display.print(lines[1].visibleLine);

        if (showNavArrows) {
            i2c_display.setCursor(0, 1);
            i2c_display.write(byte(1));
            i2c_display.setCursor(15, 1);
            i2c_display.write(byte(2));
        }
        stateChanged = false;
    }
}

void DisplayControl::setLineText(char p_text[], int p_lineIndex, enum TextAlignment p_alignment) {
    stateChanged = true;
    for (int i = 0; i < strlen(p_text); i++) lines[p_lineIndex].completeLine[i] = p_text[i];
    lines[p_lineIndex].completeLine[strlen(p_text)] = '\0' ;
    for (int i = strlen(lines[p_lineIndex].completeLine) + 1; i < 30; i++) lines[p_lineIndex].completeLine[i] = 0;
    lines[p_lineIndex].alignment = p_alignment;
}

void DisplayControl::navArrows() { showNavArrows = true; }
void DisplayControl::noNavArrows() { showNavArrows = false; }

bool DisplayControl::rested() {return (lastWrite + restDelay <= millis());}
