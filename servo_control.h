#include <Servo.h>

#include "common_stuff.h"

#ifndef CLASIFICADORA_SERVO_CONTROL_H
#define CLASIFICADORA_SERVO_CONTROL_H

class ServoControl {
public:
    ServoControl(int p_pin_servo_signal);

    void initialize();
    void moveToColor(ColorCategory p_category);

private:
    int pinSignal;
    int rotation_black;
    Servo servo_obj;
    bool initialized;
};

#endif //CLASIFICADORA_SERVO_CONTROL_H
