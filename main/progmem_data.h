#include "menu_control.h"

#ifndef GITHUB_REPO_PROGMEM_DATA_H
#define GITHUB_REPO_PROGMEM_DATA_H

const MenuItemS PROGMEM menu_01 = {1, 0, 0, "TAPINATOR 9KPP\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_02 = {2, 1, 0, "Iniciar Ya\0\0\0\0\0\0\0\0\0", 0};
const MenuItemS PROGMEM menu_03 = {3, 1, 1, "Clasificar\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_04 = {4, 1, 2, "Configurar\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_05 = {5, 1, 3, "Estadistica\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_06 = {6, 3, 0, "Automatico\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_07 = {7, 6, 0, "Comenzar\0\0\0\0\0\0\0\0\0\0\0", 0};
const MenuItemS PROGMEM menu_08 = {8, 4, 0, "Calib. disco\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_09 = {9, 8, 0, "A reloj\0\0\0\0\0\0\0\0\0\0\0\0", 5};
const MenuItemS PROGMEM menu_10 = {10, 8, 1, "Contra reloj\0\0\0\0\0\0\0", 6};
const MenuItemS PROGMEM menu_11 = {11, 4, 1, "Acerca de\0\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_12 = {12, 11, 0, "Equipo\0\0\0\0\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_13 = {13, 12, 0, "Vogel J.\0\0\0\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_14 = {14, 12, 1, "Ponce.. E.\0\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_15 = {15, 12, 2, "Jorajuria S.\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_16 = {16, 12, 3, "Jauregui R.\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_17 = {17, 11, 1, "Clase\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_18 = {18, 17, 0, "U. Catolica\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_19 = {19, 17, 1, "2021 Sem 1\0\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_20 = {20, 17, 2, "TI3 Grupo B\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_21 = {21, 17, 3, "Equipo 12\0\0\0\0\0\0\0\0\0\0", 17};
const MenuItemS PROGMEM menu_22 = {22, 3, 1, "Manual\0\0\0\0\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_23 = {23, 22, 0, "Mover disco\0\0\0\0\0\0\0\0", 4};
const MenuItemS PROGMEM menu_24 = {24, 22, 1, "Leer color\0\0\0\0\0\0\0\0\0", 2};
const MenuItemS PROGMEM menu_25 = {25, 22, 2, "Mover servo\0\0\0\0\0\0\0\0", 3};
const MenuItemS PROGMEM menu_26 = {26, 3, 2, "Paso a paso\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_27 = {27, 26, 0, "Siguiente\0\0\0\0\0\0\0\0\0\0", 1};
const MenuItemS PROGMEM menu_28 = {28, 5, 0, "Tiempo\0\0\0\0\0\0\0\0\0\0\0\0\0", 7};
const MenuItemS PROGMEM menu_29 = {29, 5, 1, "Cantidades\0\0\0\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_30 = {30, 29, 0, "Rojos\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 9};
const MenuItemS PROGMEM menu_31 = {31, 29, 1, "Azules\0\0\0\0\0\0\0\0\0\0\0\0\0", 10};
const MenuItemS PROGMEM menu_32 = {32, 29, 2, "Verdes\0\0\0\0\0\0\0\0\0\0\0\0\0", 11};
const MenuItemS PROGMEM menu_33 = {33, 29, 3, "Amarillos\0\0\0\0\0\0\0\0\0\0", 12};
const MenuItemS PROGMEM menu_34 = {34, 29, 4, "Blancos\0\0\0\0\0\0\0\0\0\0\0\0", 14};
const MenuItemS PROGMEM menu_35 = {35, 29, 5, "Grises\0\0\0\0\0\0\0\0\0\0\0\0\0", 15};
const MenuItemS PROGMEM menu_36 = {36, 29, 6, "Negros\0\0\0\0\0\0\0\0\0\0\0\0\0", 13};
const MenuItemS PROGMEM menu_37 = {37, 4, 2, "Calib. Sensor\0\0\0\0\0\0", 18};
const MenuItemS PROGMEM menu_38 = {38, 37, 0, "Leer\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 19};

const MenuItemS menu2[38] PROGMEM = {
        menu_01, menu_02, menu_03, menu_04, menu_05, menu_06, menu_07, menu_08, menu_09, menu_10,
        menu_11, menu_12, menu_13, menu_14, menu_15, menu_16, menu_17, menu_18, menu_19, menu_20,
        menu_21, menu_22, menu_23, menu_24, menu_25, menu_26, menu_27, menu_28, menu_29, menu_30,
        menu_31, menu_32, menu_33, menu_34, menu_35, menu_36, menu_37, menu_38
};

const RGBColorNorm samples[11] PROGMEM = {
        {114, 95, 75},
        {112, 95, 72},
        {160, 70, 62},
        {136, 94, 61},
        {157, 76, 49},
        {129, 81, 76},
        {152, 70, 69},
        {81, 103, 96},
        {100, 110, 71},
        {95, 110, 78},
        {126, 89, 70}
};

const char sampleNames[11][11] PROGMEM = {
        "Black\0\0\0\0\0",
        "White\0\0\0\0\0",
        "Red\0\0\0\0\0\0\0",
        "Yellow\0\0\0\0",
        "Orange\0\0\0\0",
        "Purple\0\0\0\0",
        "Pink\0\0\0\0\0\0",
        "Blue\0\0\0\0\0\0",
        "LightGreen",
        "DarkGreen\0",
        "Brown\0\0\0\0\0"
};

#endif //GITHUB_REPO_PROGMEM_DATA_H
