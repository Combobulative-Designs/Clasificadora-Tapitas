#ifndef COLOR_H
#define COLOR_H

#define MAX_HUE_ROJO 14
#define MAX_HUE_NARANJA 30
#define MAX_HUE_AMARILLO 60
#define MAX_HUE_VERDE 150
#define MAX_HUE_AZUL 270

#define INDEX_OF_NAME_BLANCO 0
#define INDEX_OF_NAME_NEGRO 1
#define INDEX_OF_NAME_GRIS 2
#define INDEX_START_OF_COLORS 3

#define LUMINOSITY_MIN_BLANCO 31
#define LUMINOSITY_MAX_NEGRO LUMINOSITY_MIN_BLANCO
#define LUMINOSITY_MAX_GRIS LUMINOSITY_MAX_NEGRO
#define SATURATION_MAX_BLANCO 11
#define SATURATION_MAX_NEGRO 20
#define SATURATION_MAX_GRIS 15

#define H_CONVERTER 360.0
#define LS_CONVERTER 100.0

#define CHAR_MAX_VAL 255

#define UNKOWN_COLOR -1

/**
 * Estructura para almacenar la informacion RGB del color
 */
typedef struct{
    float red;
    float green;
    float blue;
} RGB_t;

/**
 * Estructura para almacenar la informacion HSL del color
 */
typedef struct{
    double hue;
    double saturation;
    double luminosity;
} HSL_t;


void COLOR_init( );
char * COLOR_getColorName();
char* COLOR_getNameFromIndex( int p_index );
int COLOR_getIndex( HSL_t p_hsl );
RGB_t COLOR_getRGB( );
HSL_t COLOR_getHSLfromRGB( RGB_t p_rgb );

#endif /* COLOR_H */
