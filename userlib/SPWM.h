#ifndef __SPWM_H__
#define __SPWM_H__
#include "inc.h"

#define MasterTimer true
#define SonTimer    false

typedef struct SPWM
{
    /* data */
    HRTIM_Timerx_TypeDef    *TIM_Handle; //高分辨率定时器接口
    HRTIM_Master_TypeDef    *MasterHandle;//master 接口

    float                   SinWave_Fre; //调制得到的正弦波的频率
    float                   PWM_Fre;     //产生的PWM的频率
    double                  Step_Raw;    //单位步进 0-2*pi 每次进入中断后theta的步进
    float                   Modulation_Depth;//调制深度

    float                   theta;       //当前spwm输出的角度
    bool                    Timer_Select;
}spwm;

void SPWM_Init(spwm* spwm_mode,float Sin_Fre,float PWM_Fre,bool timerswitch);
void AUTO_SPWM(spwm* spwm_mode);
void Pessitive_SPWM(spwm* spwm_mode,float theta);

#endif


















#define __SPWM_H__