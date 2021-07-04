#include "common_stuff.h"

#include "display_control.h"
#include "sorter_control.h"

#ifndef CLASIFICADORA_TI3_MENU_CONTROL_H
#define CLASIFICADORA_TI3_MENU_CONTROL_H

struct MenuItemS {
    const int id;
    const int parentId;
    const int siblingIndex;
    const char text[20];
    const int action;
};

class MenuControl {
public:
    MenuControl(const MenuItemS (&)[36], DisplayControl (&));

    void processState();
    void initialize();
    void triggerUserAction(enum MenuUserActions p_user_action);

    enum MenuActions getCurrentAction();
    int getCurrentMenuItemId();
    MenuItemS getMenuItem(int p_id);

    int getNextSiblingId();
    int getPrevSiblingId();
    int getSiblingCount();
    int getFirstChild();

    bool locked();
    bool rested();
    bool inactive();
    bool canGoBack();

    void setLock(int p_duration);

private:
    const MenuItemS (&menu)[36];
    enum MenuActions currentAction;
    int currentMenuItemId;
    bool initialized;
    bool stateChanged;

    int lockDuration;
    unsigned long lockFrom;

    int inactivityTimer;
    int restDelay;
    unsigned long lastActivity;

    DisplayControl &displayControl;

    void printToScreenMenuItem();
};

#endif //CLASIFICADORA_TI3_MENU_CONTROL_H
