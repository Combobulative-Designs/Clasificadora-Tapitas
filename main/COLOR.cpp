
#include "COLOR.h"
#include "Adafruit_TCS34725.h"
#include "ColorConverterLib.h"

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725( TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X );

const int COLOR_HUE[] = { MAX_HUE_ROJO, MAX_HUE_NARANJA, MAX_HUE_AMARILLO, MAX_HUE_VERDE, MAX_HUE_AZUL };
const int COLOR_ARRAY_LENGTH = sizeof (COLOR_HUE ) / sizeof (int );
// Es importante que Blanco, Negro y Gris sean los primeros tres! Si se desea modificar hay que cambiar los defines "INDEX_OF"
// Los colores deben estar en el mismo orden que están en COLOR_HUE
const char* COLOR_NAME[] = { "BLANCO", "NEGRO", "GRIS", "ROJO", "NARANJA", "AMARILLO", "VERDE", "AZUL", };

/* Es necesario llamar esta función en setup() para inicializar el sensor */
void COLOR_init( ){
    colorSensor.begin( );
}

/* Esta función es la más directa de usar. Simplemente entrega el nombre del color luego de observarlo*/
char * COLOR_getColorName(){
    RGB_t colorInRGB = COLOR_getRGB();
    HSL_t colorInHSL = COLOR_getHSLfromRGB( colorInRGB );
    int index = COLOR_getIndex( colorInHSL );
    return COLOR_getNameFromIndex(index);
}

/* Esta función devuelve un nombre amigable del color que se le pasa.
 Toma como entrada el índice del color dentro de COLOR_HUE[] */
char* COLOR_getNameFromIndex( int p_index ){
    if( p_index >= 0 && p_index < sizeof(COLOR_NAME) ) return const_cast<char*> ( COLOR_NAME[ p_index ] );
    return (char*)"DESCONOCIDO";
}

/**
 * Esta función devulve el índice del array de tonos (COLOR_HUE)
 */
int COLOR_getIndex( HSL_t p_hsl ){
    // Si los parámetros caen dentro de la escala de grises
    if( p_hsl.luminosity >= LUMINOSITY_MIN_BLANCO && p_hsl.saturation < SATURATION_MAX_BLANCO ){
        return INDEX_OF_NAME_BLANCO;
    }
    if( p_hsl.luminosity < LUMINOSITY_MAX_NEGRO && p_hsl.saturation < SATURATION_MAX_NEGRO ){
        return INDEX_OF_NAME_NEGRO;
    }
    if( p_hsl.luminosity < LUMINOSITY_MAX_GRIS && p_hsl.saturation < SATURATION_MAX_GRIS ){
        return INDEX_OF_NAME_GRIS;
    }
    // Dependiendo del hue devuelve el color correspondiente del array de colores
    for( int i = 0; i < COLOR_ARRAY_LENGTH; i++ ){
        if( p_hsl.hue <= COLOR_HUE[i] ){
            return i + INDEX_START_OF_COLORS;
        }
    }
    return UNKOWN_COLOR;
}

/**
   Transforma del modelo de color RGB al modelo HSL
 */
HSL_t COLOR_getHSLfromRGB( RGB_t p_rgb ){

    double hue;
    double saturation;
    double luminosity;
    ColorConverter::RgbToHsl(
                              static_cast < uint8_t > ( p_rgb.red ),
                              static_cast < uint8_t > ( p_rgb.green ),
                              static_cast < uint8_t > ( p_rgb.blue ),
                              hue, saturation, luminosity );

    HSL_t hsl;
    hsl.hue = hue * H_CONVERTER;
    hsl.saturation = saturation * LS_CONVERTER;
    hsl.luminosity = luminosity * LS_CONVERTER;

    return hsl;
}


/**
   Obtiene el color RGB del Sensor
 */
RGB_t COLOR_getRGB( ){

    uint16_t white;
    uint16_t red;
    uint16_t green;
    uint16_t blue;

    //Apagar el led de iluminacion integrado al sensor de color
    colorSensor.setInterrupt( false );

    // Toma al menos 60ms obtener un color
    delay( 60 );

    //Obtenemos los colores del sensor
    colorSensor.getRawData( &red, &green, &blue, &white );

    //Encender el led integrado al sensor de color
    colorSensor.setInterrupt( true );

    // Obtener Codigo RGB
    RGB_t rgb;
    rgb.red = ( ( ( float ) red / white ) ) * CHAR_MAX_VAL;
    rgb.green = ( ( ( float ) green / white ) ) * CHAR_MAX_VAL;
    rgb.blue = ( ( float ) blue / white ) * CHAR_MAX_VAL;
    return rgb;
}
