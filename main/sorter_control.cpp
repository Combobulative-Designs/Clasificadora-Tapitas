#include <Arduino.h>

#include "sorter_control.h"

#include "common_stuff.h"
#include "sensor_control.h"
#include "servo_control.h"
#include "stepper_control.h"
#include "rgb_control.h"
#include "display_control.h"
#include "progmem_data.h"
#include "PROGMEM_readAnything.h"


SorterControl::SorterControl(SensorControl &p_sensorControl,
                             StepperControl &p_stepperControl,
                             ServoControl &p_servoControl,
                             SOutputControl &p_sOutputControl,
                             RGBControl &p_rgbControl,
                             DisplayControl &p_displayControl
                             ) :
                             sensorControl(p_sensorControl),
                             stepperControl(p_stepperControl),
                             servoControl(p_servoControl),
                             sOutputControl(p_sOutputControl),
                             rgbControl(p_rgbControl),
                             displayControl(p_displayControl),
                             initialized(false),
                             locked(false),
                             stopSignal(false),
                             lastAction(SorterActions::ServoTurn),
                             currentProgram(SorterPrograms::Startup),
                             currentProgramStep(1),
                             programStartTime(0),
                             colorCategoryRead(ColorCategory::Blacks),
                             lastRGBReading{0,0,0},
                             rgbSamplesForAvg{0},
                             processedTotal(0),
                             processedByColor{0,0,0,0,0,0,0},
                             avgIndex(0),
                             activeSampleArchive(0)
                             {}

void SorterControl::initialize() {
    if (!initialized) {
        sensorControl.initialize();
        stepperControl.initialize();
        servoControl.initialize();
        sOutputControl.initialize();
        rgbControl.initialize();
        displayControl.initialize();

        initialized = true;
    }
}

void SorterControl::startProgram(enum SorterPrograms p_program) {
    if (initialized and currentProgram == SorterPrograms::Rest) {
        currentProgram = p_program;
        currentProgramStep = 1;
    }
}

void SorterControl::stopProgram() {
    if (initialized and currentProgram != SorterPrograms::Rest and !locked) {
        stopSignal = true;
        displayControl.noNavArrows();
        displayControl.setLineText((char*)"Deteniendo\0\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
        displayControl.setLineText((char*)"...\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, TextAlignment::Center);
    }
}

bool SorterControl::isBusy() {
    if (initialized) {
        if (currentProgram == SorterPrograms::Rest) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

void SorterControl::processState() {
    if (initialized) {
        switch (currentProgram) {
            case SorterPrograms::Rest:
                break;
            case SorterPrograms::ManualStepper:
                PGM_ManualStepper();
                break;
            case SorterPrograms::ManualServo:
                PGM_ManualServo();
                break;
            case SorterPrograms::ManualSensor:
                PGM_ManualSensor();
                break;
            case SorterPrograms::ByStepNext:
                PGM_ByStepNext();
                break;
            case SorterPrograms::Automatic:
                PGM_Automatic();
                break;
            case SorterPrograms::CycleStepper:
                //PGM_SensorData();
                PGM_CycleStepper();
                break;
            case SorterPrograms::CalibrateSensor:
                PGM_SensorAvg();
                break;
            case SorterPrograms::Startup:
                PGM_Startup();
                break;
            default:
                break;
        }
    }
}

void SorterControl::PGM_ManualStepper() {
    if (initialized) {
        switch (currentProgramStep) {
            case 1:
                locked = true;
                programStartTime = millis();
                currentProgramStep++;
                break;
            case 2:
                displayControl.noNavArrows();
                displayControl.setLineText((char*)"Mov. disco\0\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                displayControl.setLineText((char*)"...\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, TextAlignment::Center);
                currentProgramStep++;
                break;
            case 3:
                if (!stepperControl.isBusy()) {
                    stepperControl.doCapStep();
                    currentProgramStep++;
                }
                break;
            case 4:
                if (millis() - programStartTime >= 1000) currentProgramStep++;
                break;
            case 5:
                locked = false;
                currentProgramStep = 99;
                currentProgram = SorterPrograms::Rest;
                lastAction = SorterActions::ServoTurn;
                break;
            default:
                break;
        }
    }
}

void SorterControl::PGM_ManualServo() {
    if (initialized) {
        switch (currentProgramStep) {
            case 1:
                locked = true;
                programStartTime = millis();
                currentProgramStep++;
                break;
            case 2:
                displayControl.noNavArrows();
                displayControl.setLineText((char*)"Mov. servo\0\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                displayControl.setLineText((char*)"...\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, TextAlignment::Center);
                currentProgramStep++;
                break;
            case 3:
                servoControl.moveToColor(colorCategoryRead);
                rgbControl.setColor(RGBColors::Green);
                currentProgramStep++;
                break;
            case 4:
                if (millis() - programStartTime >= 1000) currentProgramStep++;
                break;
            case 5:
                locked = false;
                rgbControl.setColor(RGBColors::Off);
                currentProgramStep = 99;
                currentProgram = SorterPrograms::Rest;
                lastAction = SorterActions::ServoTurn;
                break;
            default:
                break;
        }
    }
}

void SorterControl::PGM_ManualSensor() {
    if (initialized) {
        switch (currentProgramStep) {
            case 1:
                locked = true;
                programStartTime = millis();
                currentProgramStep++;
                break;
            case 2:
                displayControl.noNavArrows();
                displayControl.setLineText((char*)"Detectando\0\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                displayControl.setLineText((char*)"...\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, TextAlignment::Center);
                currentProgramStep++;
                break;
            case 3:
                if (!sensorControl.isBusy()) {
                    sensorControl.requestColorReading();
                    currentProgramStep++;
                }
                break;
            case 4:
                if (!sensorControl.isBusy()) {
                    lastRGBReading = sensorControl.getRGB();
                    colorCategoryRead = sensorControl.getColorCategory();
                    char rgbStr[12];
                    char sampleName[11];
                    sensorControl.getRGBStr(rgbStr);
                    sensorControl.getRGBName(sampleName);
                    displayControl.setLineText(rgbStr, 0, TextAlignment::Center);
                    displayControl.setLineText(sampleName, 1, TextAlignment::Center);
                    currentProgramStep++;
                }
                break;
            case 5:
                if (millis() - programStartTime >= 2000) currentProgramStep++;
                break;
            case 6:
                locked = false;
                currentProgramStep = 99;
                currentProgram = SorterPrograms::Rest;
                lastAction = SorterActions::ColorReading;
                break;
            default:
                break;
        }
    }
}

void SorterControl::PGM_ByStepNext() {
    if (initialized) {
        switch (currentProgramStep) {
            case 1:
                locked = true;
                programStartTime = millis();
                currentProgramStep++;
                break;
            case 2:
                displayControl.noNavArrows();
                switch (lastAction) {
                    case SorterActions::ServoTurn:
                        displayControl.setLineText((char*)"Mov. disco\0\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                        break;
                    case SorterActions::DiscStep:
                        displayControl.setLineText((char*)"Detectando\0\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                        break;
                    case SorterActions::ColorReading:
                        displayControl.setLineText((char*)"Mov. servo\0\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                        break;
                    default:
                        break;
                }
                displayControl.setLineText((char*)"...\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, TextAlignment::Center);
                currentProgramStep++;
                break;
            case 3:
                switch (lastAction) {
                    case SorterActions::ServoTurn:
                        if (!stepperControl.isBusy()) {
                            stepperControl.doCapStep();
                            currentProgramStep++;
                        }
                        break;
                    case SorterActions::DiscStep:
                        if (!sensorControl.isBusy()) {
                            sensorControl.requestColorReading();
                            currentProgramStep++;
                        }
                        break;
                    case SorterActions::ColorReading:
                        servoControl.moveToColor(colorCategoryRead);
                        rgbControl.setColor(RGBColors::Green);
                        currentProgramStep++;
                        break;
                    default:
                        break;
                }
                break;

            case 4:
                switch (lastAction) {
                    case SorterActions::ServoTurn:
                        if (!stepperControl.isBusy()) currentProgramStep++;
                        break;
                    case SorterActions::DiscStep:
                        if (!sensorControl.isBusy()) {
                            lastRGBReading = sensorControl.getRGB();
                            colorCategoryRead = sensorControl.getColorCategory();
                            currentProgramStep++;
                        }
                        break;
                    case SorterActions::ColorReading:
                        if (millis() - programStartTime >= 1000) currentProgramStep++;
                        break;
                    default:
                        break;
                }
                break;

            case 5:
                switch (lastAction) {
                    case SorterActions::ServoTurn:
                        lastAction = SorterActions::DiscStep;
                        break;
                    case SorterActions::DiscStep:
                        lastAction = SorterActions::ColorReading;
                        break;
                    case SorterActions::ColorReading:
                        rgbControl.setColor(RGBColors::Off);
                        lastAction = SorterActions::ServoTurn;
                        break;
                    default:
                        break;
                }
                locked = false;
                currentProgram = SorterPrograms::Rest;
                currentProgramStep = 99;
                break;

            default:
                break;
        }
    }
}

void SorterControl::PGM_Automatic() {
    if (initialized) {
        switch (currentProgramStep) {
            case 1:
                locked = false;
                sOutputControl.on();
                programStartTime = millis();
                currentProgramStep++;
                break;
            case 2:
                if (!stopSignal) {
                    displayControl.noNavArrows();
                    displayControl.setLineText((char*)"Clasificando\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                    displayControl.setLineText((char*)"...\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, TextAlignment::Center);
                }
                currentProgramStep++;
                break;
            case 3:
                switch (lastAction) {
                    case SorterActions::ServoTurn:
                        if (!stepperControl.isBusy()) {
                            stepperControl.doCapStep();
                            currentProgramStep++;
                        }
                        break;
                    case SorterActions::DiscStep:
                        if (!sensorControl.isBusy()) {
                            sensorControl.requestColorReading();
                            currentProgramStep++;
                        }
                        break;
                    case SorterActions::ColorReading:
                        rgbControl.setColor(RGBColors::Green);
                        servoControl.moveToColor(colorCategoryRead);
                        currentProgramStep++;
                        break;
                    default:
                        break;
                }
                break;

            case 4:
                switch (lastAction) {
                    case SorterActions::ServoTurn:
                        if (!stepperControl.isBusy()) currentProgramStep++;
                        break;
                    case SorterActions::DiscStep:
                        if (!sensorControl.isBusy()) {
                            lastRGBReading = sensorControl.getRGB();
                            colorCategoryRead = sensorControl.getColorCategory();
                            processedTotal++;
                            processedByColor[(int)colorCategoryRead]++;
                            currentProgramStep++;
                        }
                        break;
                    case SorterActions::ColorReading:
                        if (rgbControl.isRested()) {
                            currentProgramStep++;
                        }
                        break;
                    default:
                        break;
                }
                break;

            case 5:
                switch (lastAction) {
                    case SorterActions::ServoTurn:
                        lastAction = SorterActions::DiscStep;
                        break;
                    case SorterActions::DiscStep:
                        lastAction = SorterActions::ColorReading;
                        break;
                    case SorterActions::ColorReading:
                        rgbControl.setColor(RGBColors::Off);
                        lastAction = SorterActions::ServoTurn;
                        break;
                    default:
                        break;
                }
                if (stopSignal) {
                    currentProgram = SorterPrograms::Rest;
                    currentProgramStep = 99;
                    sOutputControl.off();
                    stopSignal = false;
                } else {
                    currentProgramStep = 3;
                }
                break;

            default:
                break;
        }
    }
}


void SorterControl::PGM_CycleStepper() {
    if (initialized) {
        switch (currentProgramStep) {
            case 1:
                locked = false;
                programStartTime = millis();
                currentProgramStep++;
                break;
            case 2:
                displayControl.noNavArrows();
                displayControl.setLineText((char*)"Cycling disk\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                displayControl.setLineText((char*)"...\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, TextAlignment::Center);
                currentProgramStep++;
                break;
            case 3:
                if (!stepperControl.isBusy()) {
                    stepperControl.doCycling();
                    currentProgramStep++;
                }
                break;
            case 4:
                if (millis() - programStartTime >= 1000) currentProgramStep++;
                break;
            case 5:
                if (stopSignal) {
                    stepperControl.stopActions();
                    currentProgram = SorterPrograms::Rest;
                    currentProgramStep = 99;
                    stopSignal = false;
                }
                break;
            default:
                break;
        }
    }
}

void SorterControl::PGM_Startup() {
    if (initialized) {
        switch (currentProgramStep) {
            case 1:
                locked = true;
                programStartTime = millis();
                currentProgramStep++;
                break;
            case 2:
                displayControl.noNavArrows();
                displayControl.setLineText(
                        (char*)"TAPINATOR 9KPP\0\0\0\0\0",
                        0,
                        TextAlignment::Center);
                displayControl.setLineText(
                        (char*)"Inicializando...\0\0\0",
                        1,
                        TextAlignment::Center);
                currentProgramStep++;
                break;

            case 3:
                if (!sensorControl.isBusy()) {
                    sensorControl.requestColorReading();
                    currentProgramStep++;
                }
                break;
            case 4:
                if (!sensorControl.isBusy()) {
                    lastRGBReading = sensorControl.getRGB();

                    int colorDistMin = 9999;
                    int colorDist;

                    for (int archInd = 0; archInd < 4; archInd++) {
                        RGBSampleSet activeSampleSet;
                        PROGMEM_readAnything (&samplesArchive [archInd], activeSampleSet);

                        
                        char sampleName321[12];
                        sensorControl.getRGBStr(sampleName321, activeSampleSet.base);
                        colorDist = sensorControl.getDistanceToSample(lastRGBReading, activeSampleSet.base);
                        Serial.print(sampleName321);
                        if (colorDist < colorDistMin) {
                            activeSampleArchive = archInd;
                            colorDistMin = colorDist;
                        }

                    }
                    sensorControl.setActiveSampleArchive(activeSampleArchive);
                    currentProgramStep++;
                }
                break;
            case 5:
                if (millis() - programStartTime >= 3000) currentProgramStep++;
                break;
            case 6:
                displayControl.noNavArrows();
                displayControl.setLineText(
                        (char*)"TAPINATOR 9KPP\0\0\0\0\0",
                        0,
                        TextAlignment::Center);
                displayControl.setLineText(
                        (char*)"Bienvenido\0\0\0\0\0\0\0\0\0",
                        1,
                        TextAlignment::Center);
                currentProgramStep++;
                break;
            case 7:
                if (millis() - programStartTime >= 6000) currentProgramStep++;
                break;
            case 8:
                locked = false;
                currentProgramStep = 99;
                currentProgram = SorterPrograms::Rest;
                lastAction = SorterActions::ServoTurn;
                break;
            default:
                break;
        }
    }
}

void SorterControl::PGM_SensorAvg() {
    if (initialized) {
        switch (currentProgramStep) {
            case 1:
                locked = true;
                avgIndex = 0;
                programStartTime = millis();
                currentProgramStep++;
                break;
            case 2:
                displayControl.noNavArrows();
                displayControl.setLineText((char*)"Detectando\0\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                displayControl.setLineText((char*)"...\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, TextAlignment::Center);
                currentProgramStep++;
                break;
            case 3:
                if (avgIndex < 10) {
                    if (!sensorControl.isBusy()) {
                        sensorControl.requestColorReading();
                        currentProgramStep++;
                    }
                } else {
                    for (int i = 0; i < 10; i++) {
                        lastRGBReading.red += rgbSamplesForAvg[i].red;
                    }
                    for (int i = 0; i < 10; i++) {
                        lastRGBReading.green += rgbSamplesForAvg[i].green;
                    }
                    for (int i = 0; i < 10; i++) {
                        lastRGBReading.blue += rgbSamplesForAvg[i].blue;
                    }
                    lastRGBReading.red = lastRGBReading.red / 10;
                    lastRGBReading.green = lastRGBReading.green / 10;
                    lastRGBReading.blue =  lastRGBReading.blue / 10;
                    char rgbStr[12];
                    sensorControl.getRGBStr(rgbStr, lastRGBReading);
                    displayControl.setLineText("Color", 0, TextAlignment::Center);
                    displayControl.setLineText(rgbStr, 1, TextAlignment::Center);
                    currentProgramStep = 5;
                }
                break;
            case 4:
                if (!sensorControl.isBusy()) {
                    rgbSamplesForAvg[avgIndex] = sensorControl.getRGB();
                    avgIndex++;
                    currentProgramStep = 3;
                }
                break;
            case 5:
                if (millis() - programStartTime >= 12000) currentProgramStep++;
                break;
            case 6:
                locked = false;
                currentProgramStep = 99;
                currentProgram = SorterPrograms::Rest;
                lastAction = SorterActions::ColorReading;
                break;
            default:
                break;
        }
    }
}

void SorterControl::PGM_SensorData() {
    if (initialized) {
        switch (currentProgramStep) {
            case 1:
                locked = true;
                programStartTime = millis();
                currentProgramStep++;
                break;
            case 2:
                displayControl.noNavArrows();
                displayControl.setLineText((char*)"Detectando\0\0\0\0\0\0\0\0\0", 0, TextAlignment::Center);
                displayControl.setLineText((char*)"...\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 1, TextAlignment::Center);
                currentProgramStep++;
                break;
            case 3:
                if (!sensorControl.isBusy()) {
                    sensorControl.requestColorReading();
                    currentProgramStep++;
                }
                break;
            case 4:
                if (!sensorControl.isBusy()) {
                    lastRGBReading = sensorControl.getRGB();
                    colorCategoryRead = sensorControl.getColorCategory();
                    char sampleName321[11];
                    sensorControl.getRGBName(sampleName321);
                    displayControl.setLineText("Color", 0, TextAlignment::Center);
                    displayControl.setLineText(sampleName321, 1, TextAlignment::Center);
                    currentProgramStep++;
                }
                break;
            case 5:
                if (millis() - programStartTime >= 5000) currentProgramStep++;
                break;
            case 6:
                locked = false;
                currentProgramStep = 99;
                currentProgram = SorterPrograms::Rest;
                lastAction = SorterActions::ColorReading;
                break;
            default:
                break;
        }
    }
}
