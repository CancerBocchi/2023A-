#include "SPWM.h"
#include "inc.h"

void SPWM_Init(spwm* spwm_mode,float Sin_Fre,float PWM_Fre,bool timerswitch)
{
    spwm_mode->theta = 0.0f;
    spwm_mode->PWM_Fre = PWM_Fre;
    spwm_mode->SinWave_Fre = Sin_Fre;
    spwm_mode->Step_Raw = 2.0f*PI*spwm_mode->SinWave_Fre/spwm_mode->PWM_Fre;
    spwm_mode->Modulation_Depth = 0.5f;
    spwm_mode->MasterHandle = &hhrtim1.Instance->sMasterRegs;
    spwm_mode->Timer_Select = timerswitch;
}

void AUTO_SPWM(spwm* spwm_mode)
{
    if(spwm_mode->Timer_Select == SonTimer)
        spwm_mode->TIM_Handle->CMP1xR = (uint32_t)((spwm_mode->Modulation_Depth*
                                        (arm_sin_f32(spwm_mode->theta)*0.475f+0.5f))*
                                        (float)spwm_mode->TIM_Handle->PERxR);
    else if(spwm_mode->Timer_Select == MasterTimer)
        spwm_mode->MasterHandle->MCMP1R = (uint32_t)((spwm_mode->Modulation_Depth*
                                        (arm_sin_f32(spwm_mode->theta)*0.475f+0.5f))*
                                        (float)spwm_mode->MasterHandle->MPER);
    spwm_mode->theta += spwm_mode->Step_Raw;
}

void Pessitive_SPWM(spwm* spwm_mode,float theta)
{
    spwm_mode->theta = theta;
    
    if(spwm_mode->Timer_Select == SonTimer)
        spwm_mode->TIM_Handle->CMP1xR = (uint32_t)((spwm_mode->Modulation_Depth*
                                        (arm_sin_f32(spwm_mode->theta)*0.475f+0.5f))*
                                        (float)spwm_mode->TIM_Handle->PERxR);
    else if(spwm_mode->Timer_Select == MasterTimer)
        spwm_mode->MasterHandle->MCMP1R = (uint32_t)((spwm_mode->Modulation_Depth*
                                        (arm_sin_f32(spwm_mode->theta)*0.475f+0.5f))*
                                        (float)spwm_mode->MasterHandle->MPER);

}