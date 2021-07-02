#include "common_stuff.h"

#include "display_control.h"

#ifndef CLASIFICADORA_TI3_MENU_CONTROL_H
#define CLASIFICADORA_TI3_MENU_CONTROL_H

struct MenuItemS {
    const int id;
    const int parentId;
    const int siblingIndex;
    const char text[20];
    const int action;
};

class MenuItem {
public:
    MenuItem();
    MenuItem(int, int, int, const char [20], int);

    int getId() const;
    int getParentId() const;
    int getSiblingIndex() const;
    char * getText() const;
    enum MenuActions getAction() const;

    int getNextSiblingId(const MenuItem (&)[36]) const;
    int getPrevSiblingId(const MenuItem (&)[36]) const;
    int getSiblingCount(const MenuItem (&)[36]) const;

    int getFirstChild(const MenuItem (&)[36]) const;

private:
    const int id;
    const int parentId;
    const int siblingIndex;
    const int action;
    const char text[20];
};

class MenuControl {
public:
    MenuControl(const MenuItem (&)[36], DisplayControl (&));

    void processState();
    void initialize();
    void triggerUserAction(enum MenuUserActions p_user_action);

    enum MenuActions getCurrentAction();
    int getCurrentMenuItemId();
    MenuItem getMenuItem(int p_id);

    bool locked();
    bool rested();
    bool inactive();
    bool canGoBack();

    void setLock(int p_duration);

private:
    const MenuItem (&menu)[36];
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
