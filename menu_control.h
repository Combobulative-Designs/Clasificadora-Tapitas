#include "common_stuff.h"

#include "display_control.h"

#ifndef CLASIFICADORA_TI3_MENU_CONTROL_H
#define CLASIFICADORA_TI3_MENU_CONTROL_H

class MenuItem {
public:
    MenuItem(int p_id, int p_parent_id, int p_sibling_index, char p_text[], enum MenuActions p_action);

    int getId();
    int getParentId();
    int getSiblingIndex();
    char* getText();
    enum MenuActions getAction();

    int getNextSiblingId(MenuItem p_menu[]);
    int getPrevSiblingId(MenuItem p_menu[]);
    int getSiblingCount(MenuItem p_menu[]);

    int getFirstChild(MenuItem p_menu[]);

private:
    int id;
    int parentId;
    int siblingIndex;
    char text[];
    enum MenuActions action;
};

class MenuControl {
public:
    MenuControl(MenuItem p_menu[], DisplayControl &displayControl, );

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
    MenuItem menu[];
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
    //void printToScreenOperation(bool p_cancelable);

};

#endif //CLASIFICADORA_TI3_MENU_CONTROL_H
