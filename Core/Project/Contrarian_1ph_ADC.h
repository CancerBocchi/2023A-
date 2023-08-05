#ifndef __CONTRARIAN_1PH_ADC_H__
#define __CONTRARIAN_1PH_ADC_H__

#include "inc.h"
#include "Contrarian_Defines.h"

#if Device == 1
    #define Kraw 0.000732421875f //3.0f/4096.0f
    #define Kcurrent   3.0108f
    #define Kvoltage   32.678f
    #define I_Offset 3.7548f
    #define V_Offsetout 1.2412f
    #define V_Offsetgrid 1.241f
    #define Kbus        1.0f
#elif Device == 2
    #define Kraw        0.000732421875f //3.0f/4096.0f
    #define Kcurrent    3.0291f
    #define Kvoltage    32.692f
    #define I_Offset    3.7607f
    #define V_Offset    1.24f
    #define Kbus        1.0f
#endif



typedef struct 
{
    /* data */
    float Iout;
    float Vout;
    float Vbus;
    float Vgrid;
    uint16_t Raw_Value[4];
}ADC_ValueTypeDef;

void ADC_Conversion(ADC_ValueTypeDef *ADC_Data);

#endif