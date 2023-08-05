#include "Contrarian_1ph.h"
#include "inc.h"

inline void ADC_Conversion(ADC_ValueTypeDef *ADC_Data)
{
#if Device == 1

    ADC_Data->Vout = ((float)ADC_Data->Raw_Value[0] * Kraw - V_Offset) * Kvoltage;
    ADC_Data->Iout = ((float)ADC_Data->Raw_Value[1] * Kraw ) * Kcurrent - I_Offset; 
    ADC_Data->Vbus = ((float)ADC_Data->Raw_Value[2] * Kraw * Kbus);
    ADC_Data->Vgrid = ((float)ADC_Data->Raw_Value[3] * Kraw - V_Offset) * Kvoltage;
#elif Device == 2
//
//Vout 自己输出测的电压
//Iout 自己输出测的电流
//Vbus 母线电流
//Vgird 锁相采集的电压
//
    ADC_Data->Vout = ((float)ADC_Data->Raw_Value[0] * Kraw - V_Offset) * Kvoltage;
    ADC_Data->Iout = ((float)ADC_Data->Raw_Value[1] * Kraw )* Kcurrent - I_Offset; 
    ADC_Data->Vbus = ((float)ADC_Data->Raw_Value[2] * Kraw * Kbus);
    ADC_Data->Vgrid = ((float)ADC_Data->Raw_Value[3] * Kraw - V_Offset) * Kvoltage;
#endif

    // ADC_Data->Iout *= 0.992f;//补偿环路
    // ADC_Data->Vout *= 0.99f;
    // if(ADC_Data->Vout<0)
    // {
    //     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);
    // }
    // else
    // {
    //     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
    // }

#if VALUEOUT_PRINT == ON

    //SEGGER_RTT_printf(0,"%d\n",FLOAT_PRINTF(ADC_Data->Raw_Value[3] * Kraw));

#endif

#if OFFEST_GET == ON
    static int flag;
    static float offset[2];
    if(flag < 2000)
    {
        offset[0] += ADC_Data->Raw_Value[3] * Kraw;
        offset[1] += ADC_Data->Raw_Value[1] * Kraw;
        flag ++;
    }
    if(flag == 2000)
    {
        offset[0] = offset[0]/2000.0f;
        offset[1] = offset[1]/2000.0f;
        SEGGER_RTT_printf(0,"%d,%d\n",FLOAT_PRINTF(offset[0]),FLOAT_PRINTF(offset[1]));
        flag = 0;
        offset[0] = 0.0f;
        offset[1] = 0.0f;
    }
#endif
    
}