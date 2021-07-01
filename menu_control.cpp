#include <Arduino.h>

#include "menu_control.h"

#include "display_control.h"

MenuItem::MenuItem(int p_id, int p_parent_id, int p_sibling_index, char p_text[], MenuItemActions p_action) :
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
MenuItemActions MenuItem::getAction() {return action;}

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
    lastTriggeredAction(MenuItemActions::None),
    currTriggeredAction(MenuItemActions::None),
    initialized(false),
    lockDuration(0),
    inactivityTimer(15000)
    {
        displayControl = p_displayControl
    }

bool MenuControl::locked() {
    if (lockDuration == 0) return false;
    return (millis() >= lockFrom + lockDuration);
}
bool MenuControl::resting() {
    if (inactivityTimer == 0) return inactivity;
    return (millis() >= lastActivity + inactivityTimer);
}
bool MenuControl::canGoBack() {
    return (getMenuItem(getMenuItem(getCurrMenuItemId()).getParentId()).getParentId() != 0);
}

void MenuControl::moveDown() { currMenuItemId = getMenuItem(getCurrMenuItemId()).getFirstChild(menu); }
void MenuControl::moveUp() { currMenuItemId = getMenuItem(getCurrMenuItemId()).getParentId(); }
void MenuControl::moveNext() { currMenuItemId = getMenuItem(getCurrMenuItemId()).getNextSiblingId(menu); }
void MenuControl::movePrev() { currMenuItemId = getMenuItem(getCurrMenuItemId()).getPrevSiblingId(menu); }

void MenuControl::moveTo(int p_id) {
    currMenuItemId = p_id;
}

void MenuControl::setLock(int p_duration) { lockDuration = p_duration; lockFrom = millis(); }

void MenuControl::printToScreen() {
    if (getMenuItem(getCurrMenuItem()).getSiblingCount() > 1) {
        displayControl.navArrows();
    } else {
        displayControl.noNavArrows();
    }

    displayControl.setLineText(
            getMenuItem(getMenuItem(getCurrMenuItemId()).getParentId()).getText(),
            0,
            TextAlignment::Center);
    displayControl.setLineText(
            getMenuItem(getCurrMenuItemId()).getText(),
            1,
            TextAlignment::Center);
}

MenuItemActions MenuControl::getLastTriggeredAction() { return lastTriggeredAction; }
MenuItemActions MenuControl::getCurrTriggeredAction() { return currTriggeredAction; }
int MenuControl::getLastMenuItemId() { return lastMenuItemId; }
int MenuControl::getCurrMenuItemId() { return currMenuItemId; }

MenuItem MenuControl::getMenuItem(int p_id) {
    for (const MenuItem &menuItem : menu) {
        if (menuItem.getId() == id) return menuItem;
    }
}

void MenuControl::triggerAction(MenuItemActions p_action) {
    switch (p_action) {
        case MenuItemActions::RunAuto:
            break;
        case MenuItemActions::RunStep:
            break;
        case MenuItemActions::DoSensorReading:
            break;
        case MenuItemActions::DoServoTurn:
            break;
        case MenuItemActions::DoStepperCapStep:
            break;
        case MenuItemActions::DoStepperCWCycling:
            break;
        case MenuItemActions::DoStepperCCWCycling:
            break;
        case MenuItemActions::ShowTime:
            break;
        case MenuItemActions::ShowAmountTotal:
            break;
        case MenuItemActions::ShowAmountReds:
            break;
        case MenuItemActions::ShowAmountBlues:
            break;
        case MenuItemActions::ShowAmountGreens:
            break;
        case MenuItemActions::ShowAmountYellows:
            break;
        case MenuItemActions::ShowAmountBlacks:
            break;
        case MenuItemActions::ShowAmountWhites:
            break;
        case MenuItemActions::ShowAmountGreys:
            break;
        case MenuItemActions::NavigateDown:
            moveDown();
            break;
        case MenuItemActions::None:
            break;
        default:
            break;
    }
}

void MenuControl::initialize() {
    if (!initialized) {
        Serial.print("Initializing menu.");
        lastTriggeredAction = MenuItemActions::None;
        currTriggeredAction = MenuItemActions::None;

        lastMenuItemId = 1;
        currMenuItemId = 1;

        lastActivity = millis();

        printToScreen();

        initialized = true;
    } else {
        Serial.print("Menu already initialized.")
    }
}

void MenuControl::processState() {
    if (initialized) {
        if (currMenuItemId != lastMenuItemId) {
            printToScreen()
        }








    } else {
        Serial.print("Menu not initialized.")
    }
}

