#ifndef CLASIFICADORA_COMMON_STUFF_H
#define CLASIFICADORA_COMMON_STUFF_H


enum class MenuUserActions {Next, Previous, Enter, Return};
enum class RotationDirection {Clockwise, CounterClockwise};
enum class ButtonAction {Press, Hold, Release, Rest};
enum class TextAlignment {Left, Right, Center};
enum class SensorAuxLEDMode {Off, Manual, Auto};
enum class RGBColors {
    Off,
    Red, Green, Blue ,
    Cyan, Magenta, Yellow,
    White,
    Raspberry
};
enum class SorterActions {
    DiscStep,
    ColorReading,
    ServoTurn
};
enum class SorterPrograms {
    Rest,
    ManualStepper,
    ManualServo,
    ManualSensor,
    ByStepNext,
    Automatic,
    Startup,
    CycleStepper,
    CalibrateSensor
};
enum class ColorCategory {
    Reds, Yellows, Blues,
    Greens, Whites, Greys,
    Blacks
};
enum class MenuActions {
    RunAuto, 
    RunStep, 
    DoSensorReading, 
    DoServoTurn,
    DoStepperCapStep, 
    DoStepperCWCycling, 
    DoStepperCCWCycling,
    ShowTime, 
    ShowAmountTotal, 
    ShowAmountReds, 
    ShowAmountBlues,
    ShowAmountGreens, 
    ShowAmountYellows, 
    ShowAmountBlacks,
    ShowAmountWhites, 
    ShowAmountGreys, 
    NavigateToId, 
    None,
    NavigateDown,
    ReadAndDisplayRawColor
};

typedef struct {
    int white;
    int red;
    int green;
    int blue;
} RGBColor;

typedef struct {
    int red;
    int green;
    int blue;
} RGBColorNorm;

class ButtonState {
public:
    ButtonState();

    void attach(int p_pin_button, int p_debounce_delay);
    ButtonAction getUserAction();
    bool debounced();

    void processState();

private:
    int pinButton;

    bool initialized;
    int state;
    enum ButtonAction userAction;
    unsigned long lastReading;
    int debounceDelay;
};

RGBColorNorm NormalizeRGBColor(RGBColor);
char* ConvertColorCategoryToChar(ColorCategory);
void ConvertRGBColorNormToChar(RGBColorNorm, char*);
void ConvertRGBSampleIndexToName(int, char*);

#endif //CLASIFICADORA_COMMON_STUFF_H
