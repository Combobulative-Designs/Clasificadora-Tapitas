#include "display_control.h"

#ifndef CLASIFICADORA_TI3_MENU_CONTROL_H
#define CLASIFICADORA_TI3_MENU_CONTROL_H

enum class MenuItemActions {
    RunAuto, RunStep, DoSensorReading, DoServoTurn,
    DoStepperCapStep, DoStepperCWCycling, DoStepperCCWCycling,
    ShowTime, ShowAmountTotal, ShowAmountReds, ShowAmountBlues,
    ShowAmountGreens, ShowAmountYellows, ShowAmountBlacks,
    ShowAmountWhites, ShowAmountGreys, NavigateDown, None
};
enum class MenuActions {
    Next, Previous, Enter, Cancel
};

class MenuItem {
public:
    MenuItem(int p_id, int p_parent_id, int p_sibling_index, char p_text[], MenuItemActions p_action);

    int getId();
    int getParentId();
    int getSiblingIndex();
    char* getText();
    MenuItemActions getAction();

    int getNextSiblingId(MenuItem p_menu[]);
    int getPrevSiblingId(MenuItem p_menu[]);
    int getSiblingCount(MenuItem p_menu[]);

    int getFirstChild(MenuItem p_menu[]);

private:
    int id;
    int parentId;
    int siblingIndex;
    char text[];
    MenuItemActions action;
};

class MenuControl {
public:
    MenuControl(MenuItem p_menu[], DisplayControl &displayControl);

    void processState();
    void processActions();
    void initialize();
    void triggerAction(MenuItemActions p_action);

    MenuItemActions getLastTriggeredAction();
    MenuItemActions getCurrTriggeredAction();
    int getLastMenuItemId();
    int getCurrMenuItemId();
    MenuItem getMenuItem(int p_id);

    bool locked();
    bool resting();
    bool canGoBack();

    void moveUp();
    void moveDown();
    void moveNext();
    void movePrev();

    void setLock(int p_duration);

private:
    MenuItem menu[];
    MenuItemActions lastTriggeredAction;
    MenuItemActions currTriggeredAction;
    int lastMenuItemId;
    int currMenuItemId;
    bool initialized;

    int lockDuration;
    unsigned long lockFrom;

    int inactivityTimer;
    unsigned long lastActivity;

    DisplayControl &displayControl;

    void moveTo(int p_id);
    void printToScreen();

};

#endif //CLASIFICADORA_TI3_MENU_CONTROL_H
