#include "Contrarian_1ph.h"


void HAL_HRTIM_RegistersUpdateCallback(HRTIM_HandleTypeDef * hhrtim,uint32_t TimerIdx)//周期回调
{

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)//adc转换完后进入中断
{

    ADC_Conversion(&ADC_Data);

#if LOOPOUT_PRINT == PRINPUT
    SEGGER_RTT_printf(0,"%d\n",FLOAT_PRINTF(Current_Controller.Error));
#elif LOOPOUT_PRINT == DUTYOUT
    //SEGGER_RTT_printf(0,"%d,%d\n",FLOAT_PRINTF(SPLL_Mode.sine),FLOAT_PRINTF(ADC_Data.Vout));
#elif LOOPOUT_PRINT == CURRREF
    SEGGER_RTT_printf(0,"%d\n",FLOAT_PRINTF(PR_Rms));
#endif

#if LOOPTEST == ON
    if(System_Flag_Data.Contrarian_State == System_Run)
    {
        Contrarian_Bridge_Switch(ON);
        
    #if labrun == 1
        lab1();

    #elif labrun == 2
        lab2();

    #elif labrun == 3
        lab3();
            
    #elif labrun == 4
        lab4();

    #elif labrun == 5
        lab5();

    #elif labrun == 6
        lab6();

    #endif

        //Protect_Program();

    }
    else
    {
        Contrarian_Bridge_Switch(OFF);
        SystemStop();
    }
#else
    #if Device == 1

    static uint32_t temp = 0;
    static float preValue;
    static bool looprun = false;
    if(System_Flag_Data.Contrarian_State == System_Run)
    {
        //
        //并网离网判定
        //
        if(InverterState == GridConnectionJudge)
        {  
            if(ModeDetect())//判定完标志位更改 防止继续判定
                InverterState = InverterRun;
        }
        //
        //并网离网环路run
        //
        else if(InverterState == InverterRun)
        {
            if(InverterMode == OutofGird)
            {
                Contrarian_Bridge_Switch(ON);
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
                lab5();//电压环
                LOW_C13;
            }
            else if(InverterMode == IntoGird)
            {
                SPLL_run();
                if(temp<100000)
                    temp++;
                else
                {
                    if(ADC_Data.Vgrid>0.0f && preValue<0.0f)
                    {
                        Contrarian_Bridge_Switch(ON);
                        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
                        looprun = true;
                    }
                    if(looprun)
                    {
                        Slew_Func(&PR_Rms,Keyboard_Input_Data[1],0.001f);
                        lab7();//电流环
                    }
                }
                High_C13;
            }
            Protect_Program();
            preValue = ADC_Data.Vgrid;
        }
    }
        
    else
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
        Contrarian_Bridge_Switch(OFF);
        SystemStop();
        looprun = false;
        temp = 0;
        PR_Rms = 0.0f;
        InverterMode = InitValue;
        InverterState == GridConnectionJudge;
    }

    #elif Device == 2

    static uint32_t flag = 0;
    static bool looprun = false;
    static float preVal;
    if(System_Flag_Data.Contrarian_State == System_Run)
    {
        //Inver2();

        // ThetaUpdata();

        if(flag < 100000)
            flag ++;
        else
        {
            High_C13;
            if(ADC_Data.Vgrid>0.0f && preVal<0.0f)
            {
                Contrarian_Bridge_Switch(ON);
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
                looprun = true;
            }
            if(looprun)
            {
                // LoopDutyOut = 0.12804f*SPLL_Mode.sine + 0.5f;
                // hhrtim1.Instance->sMasterRegs.MCMP1R = hhrtim1.Instance->sMasterRegs.MPER
                //                                 * LoopDutyOut;
                // if(!softstart)
                // {
                //     if(PR_Rms < 2.0f)
                //         PR_Rms += 0.00001f;
                //     else
                //         softstart = true;
                // }
                Slew_Func(&PR_Rms,1.3f,0.0001f);
                lab7();
            }
        }
        preVal = ADC_Data.Vgrid;
        SPLL_run();

        
        // lab3();
        Protect_Program();
        
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
        Contrarian_Bridge_Switch(OFF);
        SystemStop();
        looprun = false;
        flag = 0;
        PR_Rms = 0;
    }

    #endif

#endif
    HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(uint32_t)((SPLL_Mode.sine+1.1f)*1000.0f));

}