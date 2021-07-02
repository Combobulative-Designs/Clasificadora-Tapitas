#include <Arduino.h>  // for type definitions

#ifndef CLASIFICADORA_TI3_PROGMEM_READANYTHING_H
#define CLASIFICADORA_TI3_PROGMEM_READANYTHING_H

template <typename T> void PROGMEM_readAnything (const T * sce, T& dest)
{
    memcpy_P (&dest, sce, sizeof (T));
}

template <typename T> T PROGMEM_getAnything (const T * sce)
{
    static T temp;
    memcpy_P (&temp, sce, sizeof (T));
    return temp;
}

#endif //CLASIFICADORA_TI3_PROGMEM_READANYTHING_H
