#ifndef MAIN_ENCODER_CONTROL_H
#define MAIN_ENCODER_CONTROL_H

class RotaryEncoderControl {
public:
    RotaryEncoderControl();

    int getAngle();
    void attach(int p_pin_clk, int p_pin_dt);
    void processState();
    void resetRotation();

private:
    int pinCLK;
    int pinDT;
    int rotation;
    int clkLastState;
    bool initialized;
};

#endif //MAIN_ENCODER_CONTROL_H
