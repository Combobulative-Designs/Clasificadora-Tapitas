#include "common_stuff.h"

#ifndef GITHUB_REPO_RGB_CONTROL_H
#define GITHUB_REPO_RGB_CONTROL_H

class RGBControl {
public:
    RGBControl(int p_pin_red, int p_pin_green, int p_pin_blue);

    void processState();

    void initialize();
    void setColor(enum RGBColors p_color);
    bool isRested();

private:
    int pinBlue;
    int pinGreen;
    int pinRed;

    bool initialized;
    enum RGBColors color;
    bool stateChanged;
    unsigned long lastActivity;

    void writeToLED(int p_value_red, int p_value_green, int p_value_blue);
};


#endif //GITHUB_REPO_RGB_CONTROL_H
