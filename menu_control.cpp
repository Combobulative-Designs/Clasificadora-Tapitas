#include <Arduino.h>

#include "common_stuff.h"

#include "display_control.h"
#include "menu_control.h"

MenuItem::MenuItem(int p_id, int p_parent_id, int p_sibling_index, char p_text[], enum MenuItemActions p_action) :
    id(p_id),
    parentId(p_parent_id),
    siblingIndex(p_sibling_index),
    text(p_text),
    action(p_action)
{}

int MenuItem::getId() {return id;}
int MenuItem::getParentId() {return parentId;}
int MenuItem::getSiblingIndex() {return siblingIndex;}
char * MenuItem::getText() {return text;}
enum MenuItemActions MenuItem::getAction() {return action;}

int MenuItem::getSiblingCount(MenuItem p_menu[]) {
    int nodeCounter = 0;
    for (const MenuItem &menuItem : p_menu) {
        if (menuItem.getParentId() == parentId) nodeCounter++;
    }
    return nodeCounter;
}

int MenuItem::getFirstChild(MenuItem p_menu[]) {
    for (const MenuItem &menuItem : p_menu) {
        if (menuItem.getParentId() == id and menuItem.getSiblingIndex() == 0) return menuItem.getId();
    }
    return 0;
}

int MenuItem::getNextSiblingId(MenuItem p_menu[]) {
    int siblingIndexTarget;
    if (siblingIndex == getSiblingCount(p_menu) - 1) {
        siblingIndexTarget = 0;
    } else siblingIndexTarget = siblingIndex + 1;

    for (const MenuItem &menuItem : p_menu) {
        if (menuItem.getParentId() == id and menuItem.getSiblingIndex() == siblingIndexTarget) return menuItem.getId();
    }
    return id;
}

int MenuItem::getPrevSiblingId(MenuItem p_menu[]) {
    int siblingIndexTarget;
    if (siblingIndex == 0) {
        siblingIndexTarget = getSiblingCount(p_menu) - 1;
    } else siblingIndexTarget = siblingIndex - 1;

    for (const MenuItem &menuItem : p_menu) {
        if (menuItem.getParentId() == id and menuItem.getSiblingIndex() == siblingIndexTarget) return menuItem.getId();
    }
    return id;
}



MenuControl::MenuControl(MenuItem p_menu[], DisplayControl &p_displayControl) :
    menu(p_menu),
    currentAction(MenuActions::None),
    initialized(false),
    stateChanged(false),
    lockDuration(0),
    inactivityTimer(15000),
    restDelay(50)
 { displayControl = p_displayControl; }

void MenuControl::initialize() {
    if (!initialized) {
        Serial.print("Initializing menu.");

        currentMenuItemId = 1;
        lastActivity = millis();

        printToScreenMenuItem();

        initialized = true;
    } else {
        Serial.print("Menu already initialized.")
    }
}

void MenuControl::triggerUserAction(enum MenuUserActions p_user_action) {
    if (initialized) {
        if (rested() and stateChanged) {
            if (!locked()) {
                stateChanged = true;
                lastActivity = millis();
                switch (p_user_action) {
                    case MenuUserActions::Previous:
                        currentMenuItemId = getMenuItem(currentMenuItemId).getPrevSiblingId(menu);
                        currentAction = MenuActions::NavigateToId;
                        break;
                    case MenuUserActions::Next:
                        currentMenuItemId = getMenuItem(currentMenuItemId).getNextSiblingId(menu);
                        currentAction = MenuActions::NavigateToId;
                        break;
                    case MenuUserActions::Return:
                        if (currentAction == MenuActions::NavigateToId and canGoBack()) {
                            currentMenuItemId = getMenuItem(currentMenuItemId).getParentId();
                        }
                        currentAction = MenuActions::NavigateToId;
                        break;
                    default:
                        currentAction = getMenuItem(currentMenuItemId).getAction();
                        break;
                }
            } else {
                Serial.println("Menu locked.");
            }
        }
    }
    else {
        Serial.println("Menu not initialized.");
    }
}

bool MenuControl::locked() { if (lockDuration == 0) return false; return (millis() >= lockFrom + lockDuration); }
bool MenuControl::rested() { return (millis() >= lastActivity + restDelay); }
bool MenuControl::inactive() { if (inactivityTimer == 0) return false; return (millis() >= lastActivity + inactivityTimer); }
bool MenuControl::canGoBack() { return (getMenuItem(getMenuItem(currentMenuItemId).getParentId()).getParentId() != 0); }

enum MenuActions MenuControl::getCurrentAction() { return currentAction; }
int MenuControl::getCurrentMenuItemId() { return currentMenuItemId; }
MenuItem MenuControl::getMenuItem(int p_id) {
    for (const MenuItem &menuItem : menu) {
        if (menuItem.getId() == id) return menuItem;
    }
}

void MenuControl::setLock(int p_duration) { lockDuration = p_duration; lockFrom = millis(); }

void MenuControl::printToScreenMenuItem() {
    if (getMenuItem(currentMenuItemId).getSiblingCount() > 1) {
        displayControl.navArrows();
    } else {
        displayControl.noNavArrows();
    }

    displayControl.setLineText(
            getMenuItem(getMenuItem(currentMenuItemId).getParentId()).getText(),
            0,
            TextAlignment::Center);
    displayControl.setLineText(
            getMenuItem(currentMenuItemId).getText(),
            1,
            TextAlignment::Center);
}

void MenuControl::processState() {
    if (initialized) {
        if (stateChanged) {
            lastActivity = millis();
            switch (currentAction) {
                case MenuActions::RunAuto:
                    displayControl.noNavArrows();
                    displayControl.setLineText("Clasificando", 0, TextAlignment::Center);
                    displayControl.setLineText("**", 0, TextAlignment::Center);
                    break;
                case MenuActions::RunStep:
                    displayControl.noNavArrows();
                    displayControl.setLineText("Un paso...", 0, TextAlignment::Center);
                    displayControl.setLineText("**", 0, TextAlignment::Center);
                    break;
                case MenuActions::DoSensorReading:
                    break;
                case MenuActions::DoServoTurn:
                    break;
                case MenuActions::DoStepperCapStep:
                    break;
                case MenuActions::DoStepperCWCycling:
                    break;
                case MenuActions::DoStepperCCWCycling:
                    break;
                case MenuActions::ShowTime:
                    displayControl.noNavArrows();
                    displayControl.setLineText("Tiempo total:", 0, TextAlignment::Center);
                    displayControl.setLineText((char *)(milis() / 1000), 0, TextAlignment::Center);
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
                default:
                    break;
            }
        }
    } else {
        Serial.println("Menu not initialized.");
    }
}

