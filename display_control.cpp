#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "common_stuff.h"

#include "display_control.h"

DisplayControl::DisplayControl(byte p_address, int p_columns, int p_rows) : initialized(false), i2c_display(p_address, p_columns, p_rows), rows(p_rows), columns(p_columns) {}
TextLine::TextLine() : alignment(TextAlignment::Left) {};

void DisplayControl::initialize() {
    if (!initialized) {
        Serial.println("Initializing display..");
        i2c_display.init();
        i2c_display.backlight();
        i2c_display.clear();
        i2c_display.setCursor(0, 0);
        i2c_display.print("Initializing...");
        initialized = true;
    } else {
        Serial.println("Display already initialized.");
    }
}

int TextLine::getTotalCycles() {
    //int lineLen = strlen(completeLine);
    if (strlen(completeLine) > 16) {
        return marqueeDelayAfter + marqueeDelayBefore + ((strlen(completeLine) - 15) * marqueeSpeed);
    } else {
        return 0;
    }
}

void DisplayControl::processState() {
    for (int i = 0; i < 1; i++) {
        if (lines[i].completeLine != lines[i].oldCompleteLine) {
            char* newLine;

            lines[i].oldCompleteLine = lines[i].completeLine;

            lines[i].marqueeDelayBefore = 0;
            lines[i].marqueeDelayAfter = 0;
            lines[i].marqueeSpeed = 300;
            lines[i].marqueeCurrentIndex = 0;
            lines[i].marqueeCurrentCycle = 0;

            int lineLen = strlen(lines[i].completeLine);
            if (lineLen == 16) {
                newLine = lines[i].completeLine;
            } else if (lineLen < 16) {
                int spacesLeft = 16 - lineLen;
                switch (lines[i].alignment) {
                    case TextAlignment::Left:
                        sprintf(newLine, "%-16s", lines[i].completeLine);
                        break;
                    case TextAlignment::Right:
                        sprintf(newLine, "%16s", lines[i].completeLine);
                        break;
                    default:
                        int spacesBefore = spacesLeft / 2;
                        int spacesAfter = 16 - (spacesBefore + lineLen);
                        sprintf(newLine, "%*s%s%*s", spacesBefore, "", lines[i].completeLine, spacesAfter, "");
                        break;
                }
            } else {
                sprintf(newLine, "%.*s", 10, lines[i].completeLine);
            }


            if (lines[i].visibleLine != newLine) {
                i2c_display.setCursor(0, i);
                i2c_display.print(newLine);
                lines[i].visibleLine = newLine;
            }
        } else {

        }
    }
}

void DisplayControl::setLineText(char* p_text, int p_lineIndex, TextAlignment p_alignment) {
    lines[p_lineIndex].completeLine = p_text;
    lines[p_lineIndex].alignment = p_alignment;
}
