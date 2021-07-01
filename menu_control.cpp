#include "menu_control.h"

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

MenuControl::MenuControl(MenuItem p_menu[]) :
    menu(p_menu),
    lastTriggeredAction(MenuItemActions::None),
    currTriggeredAction(MenuItemActions::None),
    initialized(false);
{}


