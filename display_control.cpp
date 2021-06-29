#include <LiquidCrystal_I2C.h>

#include "common_stuff.h"

#include "display_control.h"

Display::Display(byte address, int columns, int rows) : initialized(false), i2c_display(address, columns, rows) {}
TextLine::TextLine() : alignment(TextAlignment::Left) {};

void Display::initialize() {
    if (!initialized) {
        Serial.println("Initializing display..");
        i2c_display.init();
        i2c_display.backlight();
        initialized = true;
    } else {
        Serial.println("Display already initialized.");
    }
}

int TextLine::getTotalCycles() {
    int lineLen = strlen(completeLine);
    if (lineLen > 16) {
        return marqueeDelayAfter + marqueeDelayBefore + ((lineLen - 15) * marqueeSpeed);
    } else {
        return 0;
    }
}

void Display::processState() {
    for (int i = 0; i <= 1; i++) {
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
                        //sprintf(newLine, "%-16s", lines[i].completeLine);
                        break;
                    case TextAlignment::Right:
                        //sprintf(newLine, "%16s", lines[i].completeLine);
                        break;
                    default:
                        int spacesBefore = spacesLeft / 2;
                        int spacesAfter = 16 - (spacesBefore + lineLen);
                        //sprintf(newLine, "%*s%s%*s", spacesBefore, "", lines[i].completeLine, spacesAfter, "");
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

void Display::setLineText(char* p_text, int p_lineIndex, TextAlignment p_alignment) {
    lines[p_lineIndex].completeLine = p_text;
    lines[p_lineIndex].alignment = p_alignment;
}