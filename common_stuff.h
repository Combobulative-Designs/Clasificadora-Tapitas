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

    void attach(int p_pin_button);
    int getState();
    ButtonAction getUserAction();
    int getCyclesHeld();

    void processState();

private:
    int pinButton;

    bool initialized;
    int state;
    enum ButtonAction userAction;
    int cyclesHeld;
};

void I2CScanner();
char* ConvertColorCategoryToChar(ColorCategory category);
char* RepeatChar(char p_char, int p_repetitions);

#endif //CLASIFICADORA_COMMON_STUFF_H
