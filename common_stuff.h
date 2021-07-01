#ifndef CLASIFICADORA_COMMON_STUFF_H
#define CLASIFICADORA_COMMON_STUFF_H

enum class RotationDirection {Clockwise, CounterClockwise};
enum class ButtonAction {Press, Hold, Release, Rest};
enum class TextAlignment {Left, Right, Center};
enum class ColorCategory {
    Reds, Yellows, Blues,
    Greens, Whites, Greys,
    Blacks
};

class ButtonState {
public:
    ButtonState();

    void attach(int p_pin_button, int p_debounce_delay);
    int getState();
    ButtonAction getUserAction();
    int getCyclesHeld();
    unsigned long getLastReading();
    bool debounced();

    void processState();

private:
    int pinButton;

    bool initialized;
    int state;
    enum ButtonAction userAction;
    int cyclesHeld;
    unsigned long lastReading;
    int debounceDelay;
};

int I2CScanner(byte addrs[]);
char* ConvertColorCategoryToChar(ColorCategory category);

#endif //CLASIFICADORA_COMMON_STUFF_H
