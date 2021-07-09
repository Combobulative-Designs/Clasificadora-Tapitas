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
    int red;
    int green;
    int blue;
} RGBColor;

typedef struct {
    RGBColor colors[12];
    RGBColor base;
} RGBSampleSet;

class ButtonState {
public:
    ButtonState();

    void attach(int, int);
    void processState();
    bool debounced();
    ButtonAction getUserAction();


private:
    int pinButton;
    int state;
    int debounceDelay;
    bool initialized;
    unsigned long lastReading;
    enum ButtonAction userAction;
};

enum ColorCategory RGBColorSampleIndexToCategory(int);

#endif //CLASIFICADORA_COMMON_STUFF_H
