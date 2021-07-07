#include <Arduino.h>

#ifndef GITHUB_REPO_SIMPLE_OUTPUT_CONTROL_H
#define GITHUB_REPO_SIMPLE_OUTPUT_CONTROL_H

class SOutputControl {
public:
    SOutputControl(int p_pin_signal);

    void processState();
    void initialize();
    void on();
    void off();

private:
    bool initialized;
    int pinSignal;
    bool stateChanged;
    int currentState;
};

#endif //GITHUB_REPO_SIMPLE_OUTPUT_CONTROL_H
