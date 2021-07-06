#include <Arduino.h>
#include "PROGMEM_readAnything.h"

#include "common_stuff.h"

#include "display_control.h"
#include "sorter_control.h"
#include "menu_control.h"


MenuControl::MenuControl(const MenuItemS (&p_menu)[36], DisplayControl &p_displayControl, SorterControl &p_sorterControl) :
    initialized(false),
    stateChanged(false),
    inactivityTimer(15000),
    displayControl(p_displayControl),
    sorterControl(p_sorterControl),
    menu(p_menu)
    {}

void MenuControl::initialize() {
    if (!initialized) {
        currentMenuItemId = 2;
        lastActivity = millis();

        sorterControl.initialize();
        isSorterBusy = sorterControl.isBusy();

        initialized = true;
    }
}

void MenuControl::triggerUserAction(enum MenuUserActions p_user_action) {
    if (initialized) {
        stateChanged = true;
        lastActivity = millis();
        switch (p_user_action) {
            case MenuUserActions::Previous:
                currentMenuItemId = getPrevSiblingId();
                currentAction = MenuActions::NavigateToId;
                break;
            case MenuUserActions::Next:
                currentMenuItemId = getNextSiblingId();
                currentAction = MenuActions::NavigateToId;
                break;
            case MenuUserActions::Return:
                if (sorterControl.isBusy()) {
                    sorterControl.stopProgram();
                } else {
                    currentAction = MenuActions::NavigateToId;
                    if (canGoBack()) {
                        currentMenuItemId = getMenuItem(currentMenuItemId).parentId;
                    }
                }
                break;
            default:
                currentAction = (MenuActions)getMenuItem(currentMenuItemId).action;
                if (currentAction == MenuActions::None) stateChanged = false;
                break;
        }
    }
}

bool MenuControl::inactive() { if (inactivityTimer == 0) return false; return (millis() >= lastActivity + inactivityTimer); }
bool MenuControl::canGoBack() { return (getMenuItem(currentMenuItemId).parentId != 1); }

int MenuControl::getSiblingCount() {
    int nodeCounter = 0;
    for (int i = 0; i < 36; i++) {
        MenuItemS menuItem = {0, 0, 0, "asdasdasdasdasdasda", 0};
        PROGMEM_readAnything (&menu [i], menuItem);
        if (menuItem.parentId == getMenuItem(getCurrentMenuItemId()).parentId) nodeCounter++;
    }
    return nodeCounter;
}

int MenuControl::getFirstChild() {
    for (int i = 0; i < 36; i++) {
        MenuItemS menuItem = {0, 0, 0, "asdasdasdasdasdasda", 0};
        PROGMEM_readAnything (&menu [i], menuItem);
        if (menuItem.parentId == getCurrentMenuItemId() and menuItem.siblingIndex == 0) return menuItem.id;
    }
    return 1;
}

int MenuControl::getNextSiblingId() {
    if (getMenuItem(getCurrentMenuItemId()).siblingIndex == getSiblingCount() - 1) {
        for (int i = 0; i < 36; i++) {
            MenuItemS menuItem = {0, 0, 0, "asdasdasdasdasdasda", 0};
            PROGMEM_readAnything (&menu [i], menuItem);
            if (menuItem.parentId == getMenuItem(getCurrentMenuItemId()).parentId and menuItem.siblingIndex == 0) return menuItem.id;
        }
    } else {
        for (int i = 0; i < 36; i++) {
            MenuItemS menuItem = {0, 0, 0, "asdasdasdasdasdasda", 0};
            PROGMEM_readAnything (&menu [i], menuItem);
            if (menuItem.parentId == getMenuItem(getCurrentMenuItemId()).parentId and menuItem.siblingIndex == getMenuItem(getCurrentMenuItemId()).siblingIndex + 1) return menuItem.id;
        }
    }
    return getCurrentMenuItemId();
}

int MenuControl::getPrevSiblingId() {
    if (getMenuItem(getCurrentMenuItemId()).siblingIndex == 0) {
        for (int i = 0; i < 36; i++) {
            MenuItemS menuItem = {0, 0, 0, "asdasdasdasdasdasda", 0};
            PROGMEM_readAnything (&menu [i], menuItem);
            if (menuItem.parentId == getMenuItem(getCurrentMenuItemId()).parentId and menuItem.siblingIndex == getSiblingCount() - 1) return menuItem.id;
        }
    } else {
        for (int i = 0; i < 36; i++) {
        MenuItemS menuItem = {0, 0, 0, "asdasdasdasdasdasda", 0};
        PROGMEM_readAnything (&menu [i], menuItem);
            if (menuItem.parentId == getMenuItem(getCurrentMenuItemId()).parentId and menuItem.siblingIndex == getMenuItem(getCurrentMenuItemId()).siblingIndex - 1) return menuItem.id;
        }
    }
    return getCurrentMenuItemId();
}

enum MenuActions MenuControl::getCurrentAction() { return currentAction; }
int MenuControl::getCurrentMenuItemId() { return currentMenuItemId; }
MenuItemS MenuControl::getMenuItem(int p_id) {
    for (int i = 0; i < 36; i++) {
        MenuItemS menuItem = {0, 0, 0, "asdasdasdasdasdasda", 0};
        PROGMEM_readAnything (&menu [i], menuItem);
        if (menuItem.id == p_id) return menuItem;
    }
}

void MenuControl::printToScreenMenuItem() {
    if (getSiblingCount() > 1) {
        displayControl.navArrows();
    } else {
        displayControl.noNavArrows();
    }

    displayControl.setLineText(
            (char*)getMenuItem(getMenuItem(currentMenuItemId).parentId).text,
            0,
            TextAlignment::Center);
    displayControl.setLineText(
            (char*)getMenuItem(currentMenuItemId).text,
            1,
            TextAlignment::Center);
}

void MenuControl::processState() {
    if (initialized) {
        if (isSorterBusy != sorterControl.isBusy()) {
            isSorterBusy = sorterControl.isBusy();

            if (!isSorterBusy) {
                stateChanged = true;
                currentAction = MenuActions::NavigateToId;
            }
        }

        if (stateChanged) {
            lastActivity = millis();
            stateChanged = false;
            switch (currentAction) {
                case MenuActions::RunAuto:
                    sorterControl.startProgram(SorterPrograms::Automatic);
                    break;
                case MenuActions::RunStep:
                    sorterControl.startProgram(SorterPrograms::ByStepNext);
                    break;
                case MenuActions::DoSensorReading:
                    sorterControl.startProgram(SorterPrograms::ManualSensor);
                    break;
                case MenuActions::DoServoTurn:
                    sorterControl.startProgram(SorterPrograms::ManualServo);
                    break;
                case MenuActions::DoStepperCapStep:
                    sorterControl.startProgram(SorterPrograms::ManualStepper);
                    break;
                case MenuActions::DoStepperCWCycling:
                    sorterControl.startProgram(SorterPrograms::CycleStepper);
                    break;
                case MenuActions::DoStepperCCWCycling:
                    sorterControl.startProgram(SorterPrograms::CycleStepper);
                    break;
                case MenuActions::ShowTime:
                    break;
                case MenuActions::ShowAmountTotal:
                    break;
                case MenuActions::ShowAmountReds:
                    break;
                case MenuActions::ShowAmountBlues:
                    break;
                case MenuActions::ShowAmountGreens:
                    break;
                case MenuActions::ShowAmountYellows:
                    break;
                case MenuActions::ShowAmountBlacks:
                    break;
                case MenuActions::ShowAmountWhites:
                    break;
                case MenuActions::ShowAmountGreys:
                    break;
                case MenuActions::NavigateToId:
                    printToScreenMenuItem();
                    break;
                case MenuActions::NavigateDown:
                    currentMenuItemId = getFirstChild();
                    printToScreenMenuItem();
                    break;
                default:
                    break;
            }
        }
    }
}
