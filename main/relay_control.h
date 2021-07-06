#include <Arduino.h>

#ifndef GITHUB_REPO_RELAY_CONTROL_H
#define GITHUB_REPO_RELAY_CONTROL_H

class RelayControl {
public:
    RelayControl(int p_pin_signal);

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

#endif //GITHUB_REPO_RELAY_CONTROL_H
