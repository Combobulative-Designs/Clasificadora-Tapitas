#ifndef CLASIFICADORA_TI3_MENU_CONTROL_H
#define CLASIFICADORA_TI3_MENU_CONTROL_H

enum class MenuItemActions {
    RunAuto, RunStep, DoSensorReading, DoServoTurn,
    DoStepperCapStep, DoStepperCWCycling, DoStepperCCWCycling,
    ShowTime, ShowAmountTotal, ShowAmountReds, ShowAmountBlues,
    ShowAmountGreens, ShowAmountYellows, ShowAmountBlacks,
    ShowAmountWhites, ShowAmountGreys, None
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
    MenuControl(MenuItem p_menu[]);

    void processState();
    void initialize();

    MenuItemActions getLastTriggeredAction();
    MenuItemActions getCurrTriggeredAction();
    int getLastMenuItemId();
    int getCurrMenuItemId();
    MenuItem getMenuItem(int p_id);

    void moveUp();
    void moveDown();
    void moveNext();
    void movePrev();

private:
    MenuItem menu[];
    MenuItemActions lastTriggeredAction;
    MenuItemActions currTriggeredAction;
    int lastMenuItemId;
    int currMenuItemId;
    bool initialized;

    void moveTo(int p_id);

};

#endif //CLASIFICADORA_TI3_MENU_CONTROL_H
