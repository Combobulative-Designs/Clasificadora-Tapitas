#include <Arduino.h>
#include "PROGMEM_readAnything.h"

#include "common_stuff.h"

#include "display_control.h"
#include "menu_control.h"

/*MenuItem::MenuItem() :
        id(0),
        parentId(0),
        siblingIndex(0),
        action(17),
        text{0}
{}
MenuItem::MenuItem(int p_id, int p_parent_id, int p_sibling_index, const char p_text[20], int p_action) :
        id(p_id),
        parentId(p_parent_id),
        siblingIndex(p_sibling_index),
        action(p_action),
        text{0}
{}*/

/*int MenuItem::getId() const {return id;}
int MenuItem::getParentId() const {return parentId;}
int MenuItem::getSiblingIndex() const {return siblingIndex;}
enum MenuActions MenuItem::getAction() const {return (enum MenuActions)action;}
char * MenuItem::getText() const {
  char * returnValue;
  for (int i = 0; strlen(text); i++) returnValue[i] = text[i];
  returnValue[strlen(text)] = 0;
  return returnValue;
}*/

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



MenuControl::MenuControl(const MenuItemS (&p_menu)[36], DisplayControl &p_displayControl) :
    initialized(false),
    stateChanged(false),
    lockDuration(0),
    inactivityTimer(15000),
    restDelay(50),
    displayControl(p_displayControl),
    menu(p_menu)
{}

void MenuControl::initialize() {
    if (!initialized) {
        //Serial.print(F("Initializing menu."));

        currentMenuItemId = 2;
        lastActivity = millis();

        printToScreenMenuItem();

        initialized = true;
    } else {
        //Serial.print(F("Menu already initialized."));
    }
}

void MenuControl::triggerUserAction(enum MenuUserActions p_user_action) {
    if (initialized) {
        if (rested()) {
            if (!locked()) {
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
                        currentAction = MenuActions::NavigateToId;
                        if (canGoBack()) {
                            currentMenuItemId = getMenuItem(currentMenuItemId).parentId;
                        }
                        break;
                    default:
                        currentAction = (MenuActions)getMenuItem(currentMenuItemId).action;
                        if (currentAction == MenuActions::None) stateChanged = false;
                        break;
                }
            } else {
                //Serial.println(F("Menu locked."));
            }
        }
    }
    else {
        //Serial.println(F("Menu not initialized."));
    }
}

bool MenuControl::locked() { if (lockDuration == 0) return false; return (millis() >= lockFrom + lockDuration); }
bool MenuControl::rested() { return (millis() >= lastActivity + restDelay); }
bool MenuControl::inactive() { if (inactivityTimer == 0) return false; return (millis() >= lastActivity + inactivityTimer); }
bool MenuControl::canGoBack() { return (getMenuItem(currentMenuItemId).parentId != 1); }

enum MenuActions MenuControl::getCurrentAction() { return currentAction; }
int MenuControl::getCurrentMenuItemId() { return currentMenuItemId; }
MenuItemS MenuControl::getMenuItem(int p_id) {
    for (int i = 0; i < 36; i++) {
        MenuItemS menuItem = {0, 0, 0, "asdasdasdasdasdasda", 0};
        PROGMEM_readAnything (&menu [i], menuItem);
        if (menuItem.id == p_id) return menuItem;
    }
}

void MenuControl::setLock(int p_duration) { lockDuration = p_duration; lockFrom = millis(); }

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
        if (stateChanged) {
            lastActivity = millis();
            stateChanged = false;
            switch (currentAction) {
                case MenuActions::RunAuto:
                    displayControl.noNavArrows();
                    displayControl.setLineText((char*)F("Clasificando"), 0, TextAlignment::Center);
                    displayControl.setLineText((char*)F("**"), 0, TextAlignment::Center);
                    break;
                case MenuActions::RunStep:
                    displayControl.noNavArrows();
                    displayControl.setLineText((char*)F("Un paso..."), 0, TextAlignment::Center);
                    displayControl.setLineText((char*)F("**"), 0, TextAlignment::Center);
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
                    displayControl.setLineText((char *)F("Tiempo total:"), 0, TextAlignment::Center);
                    displayControl.setLineText((char *)(millis() / 1000), 0, TextAlignment::Center);
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
    } else {
        //Serial.println(F("Menu not initialized."));
    }
}
