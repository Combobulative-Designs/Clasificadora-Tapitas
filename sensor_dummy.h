#include "common_stuff.h"

#ifndef CLASIFICADORA_SENSOR_DUMMY_H
#define CLASIFICADORA_SENSOR_DUMMY_H

#define ALT_MAX_HUE_ROJO 15
#define ALT_MAX_HUE_NARANJA 30
#define ALT_MAX_HUE_AMARILLO 60
#define ALT_MAX_HUE_VERDE 150
#define ALT_MAX_HUE_AZUL 270

#define ALT_LUMINOSITY_MIN_BLANCO 31
#define ALT_LUMINOSITY_MAX_NEGRO ALT_LUMINOSITY_MIN_BLANCO
#define ALT_LUMINOSITY_MAX_GRIS ALT_LUMINOSITY_MAX_NEGRO
#define ALT_SATURATION_MAX_BLANCO 11
#define ALT_SATURATION_MAX_NEGRO 20
#define ALT_SATURATION_MAX_GRIS 15
/*
#define ALT_H_CONVERTER 360.0
#define ALT_LS_CONVERTER 100.0
*/
enum ColorCategory ReadColorDummy();
void InitSensor();
enum ColorCategory ReadColor();

#endif //CLASIFICADORA_SENSOR_DUMMY_H
