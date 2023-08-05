#include "Contrarian_1ph.h"

//电压期望PI 电流瞬时PI
void lab1()
{
    Voltage_Controller.Ref = Keyboard_Input_Data[0];

    VoltLoop_Flag++;
    CurrLoop_Flag++;

    Mea_Mode.v = ADC_Data.Vout;
    Mea_Mode.i = ADC_Data.Iout;
    POWER_MEAS_SINE_ANALYZER_run(&Mea_Mode);

    VrmsFliter += Mea_Mode.vRms;

    if(VoltLoop_Flag == VoltFlag)
    {
        VrmsFliter = VrmsFliter/(float)VoltFlag;
        PR_Rms = Pos_PID_Controller(&Voltage_Controller,VrmsFliter);
        VrmsFliter = 0.0f;
        VoltLoop_Flag = 0;
    }

    if(CurrLoop_Flag == CurrFlag)
    {
        ThetaUpdata();
        Current_Controller.Ref = PR_Rms * SQRT_2 * sinf(PR_theta);
        LoopDutyOut = Pos_PID_Controller(&Current_Controller,ADC_Data.Iout) + 0.5f;
        //标志位清零
        CurrLoop_Flag = 0;   
    }

    //LoopDutyOut = 0.3f*sin(PR_theta)+0.5f;
    hhrtim1.Instance->sMasterRegs.MCMP1R = hhrtim1.Instance->sMasterRegs.MPER
                                        * LoopDutyOut;

}

//电压电流都为瞬时PI
void lab2()
{
    VoltLoop_Flag++;
    CurrLoop_Flag++;

    PR_Rms = Keyboard_Input_Data[0];

    if(VoltLoop_Flag == VoltFlag)
    {
        ThetaUpdata();
        Voltage_Controller.Ref = sin(PR_theta) * PR_Rms * SQRT_2;
        Current_Controller.Ref = Pos_PID_Controller(&Voltage_Controller,ADC_Data.Vout);
        VoltLoop_Flag = 0;
    }

    if(CurrLoop_Flag == CurrFlag)
    {
        LoopDutyOut = Pos_PID_Controller(&Current_Controller,ADC_Data.Iout) + 0.5f;
        //标志位清零
        CurrLoop_Flag = 0;   
    }
    hhrtim1.Instance->sMasterRegs.MCMP1R = hhrtim1.Instance->sMasterRegs.MPER
                                                * LoopDutyOut;
}

//只跑电流环
void lab3()
{
    CurrLoop_Flag++;

    PR_Rms = 1.2f;

    if(CurrLoop_Flag == CurrFlag)
    {
        ThetaUpdata();
        Current_Controller.Ref = PR_Rms * SQRT_2 * sinf(PR_theta);
        LoopDutyOut = Pos_PID_Controller(&Current_Controller,ADC_Data.Iout)+ 0.5f;
        //标志位清零
        CurrLoop_Flag = 0;   
    }
    hhrtim1.Instance->sMasterRegs.MCMP1R = hhrtim1.Instance->sMasterRegs.MPER
                                        * LoopDutyOut;
}

//角度更新与电压环同频
void lab4()
{
    VoltLoop_Flag++;
    CurrLoop_Flag++;

    PR_Rms = Keyboard_Input_Data[1];

    if(VoltLoop_Flag == VoltFlag)
    {
        ThetaUpdata();
        VoltLoop_Flag = 0;
    }
    if(CurrLoop_Flag == CurrFlag)
    { 
        Current_Controller.Ref = PR_Rms * SQRT_2 * sin(PR_theta);
        LoopDutyOut = Pos_PID_Controller(&Current_Controller,ADC_Data.Iout) + 0.5f;
        //标志位清零
        CurrLoop_Flag = 0;   
    }
    hhrtim1.Instance->sMasterRegs.MCMP1R = hhrtim1.Instance->sMasterRegs.MPER
                                        * LoopDutyOut;
}

//调制度电压环
void lab5()
{
        Mea_Mode.v = ADC_Data.Vout;
        Mea_Mode.i = ADC_Data.Iout;
        POWER_MEAS_SINE_ANALYZER_run(&Mea_Mode);
        VrmsFliter += Mea_Mode.vRms;
        VoltLoop_Flag++;
        ThetaUpdata();
        LoopDutyOut =  MD * sin(PR_theta) * 0.48f + 0.5f;
        if(VoltLoop_Flag == VoltFlag)
        {
            VrmsFliter = VrmsFliter/(float)VoltFlag;
            MD = Pos_PID_Controller(&OpenVolt_Controller,VrmsFliter);
            VrmsFliter = 0.0f;
            VoltLoop_Flag = 0;
        }
        
        hhrtim1.Instance->sMasterRegs.MCMP1R = hhrtim1.Instance->sMasterRegs.MPER
                                        * LoopDutyOut;
}

void lab6()
{
    static bool currentflag;

    Voltage_Controller.Ref = Keyboard_Input_Data[0];
    OpenVolt_Controller.Ref = Keyboard_Input_Data[0] - 0.2f;

    VoltLoop_Flag++;
    CurrLoop_Flag++;

    Mea_Mode.v = ADC_Data.Vout;
    Mea_Mode.i = ADC_Data.Iout;
    POWER_MEAS_SINE_ANALYZER_run(&Mea_Mode);

    VrmsFliter += Mea_Mode.vRms;

    if(!currentflag)
    {
        if(ADC_Data.Iout<0.1f)
            currentflag = true;
    }
    else
    {
        if(ADC_Data.Iout>0.5f)
            currentflag = false;
    }
    
    
    if(currentflag == false)
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);
        if(VoltLoop_Flag == VoltFlag)
        {
            VrmsFliter = VrmsFliter/(float)VoltFlag;
            PR_Rms = Pos_PID_Controller(&Voltage_Controller,VrmsFliter);
            VrmsFliter = 0.0f;
            VoltLoop_Flag = 0;
        }

        if(CurrLoop_Flag == CurrFlag)
        {
            ThetaUpdata();
            Current_Controller.Ref = PR_Rms * SQRT_2 * sinf(PR_theta);
            LoopDutyOut = Pos_PID_Controller(&Current_Controller,ADC_Data.Iout) + 0.5f;
            //标志位清零
            CurrLoop_Flag = 0;   
        }
    }
    else if(currentflag == true)
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
        ThetaUpdata();
       if(VoltLoop_Flag == VoltFlag)
        {
            VrmsFliter = VrmsFliter/(float)VoltFlag;
            LoopDutyOut = 0.475f * Pos_PID_Controller(&Voltage_Controller,VrmsFliter) * sin(PR_theta)+0.5f;
            VrmsFliter = 0.0f;
            VoltLoop_Flag = 0;
        }
    }


    hhrtim1.Instance->sMasterRegs.MCMP1R = hhrtim1.Instance->sMasterRegs.MPER
                                        * LoopDutyOut;
}

//电流环
void lab7()
{
    CurrLoop_Flag++;

    if(CurrLoop_Flag == CurrFlag)
    {
        //Slew_Func(&PR_Rms,0.0f,0.000001f);

        
        Current_Controller.Ref = PR_Rms * SQRT_2 * SPLL_Mode.sine;
        
        LoopDutyOut = Pos_PID_Controller(&Current_Controller,ADC_Data.Iout) + 0.5f;

        // if(LoopDutyOut > 0.9f)
        //     LoopDutyOut = 0.9f;
        // else if(LoopDutyOut < 0.1f)
        //     LoopDutyOut = 0.1f;
        
        //标志位清零
        CurrLoop_Flag = 0;   
    }
    hhrtim1.Instance->sMasterRegs.MCMP1R = hhrtim1.Instance->sMasterRegs.MPER
                                        * LoopDutyOut;

}

//调制度电压环
void lab8()
{
        Mea_Mode.v = ADC_Data.Vout;
        POWER_MEAS_SINE_ANALYZER_run(&Mea_Mode);
        VrmsFliter += Mea_Mode.vRms;
        VoltLoop_Flag++;

        if(VoltLoop_Flag == VoltFlag)
        {
            VrmsFliter = VrmsFliter/(float)VoltFlag;
            LoopDutyOut = Pos_PID_Controller(&OpenVolt_Controller,VrmsFliter) * SPLL_Mode.sine * 0.48f + 0.5f;
            VrmsFliter = 0.0f;
            VoltLoop_Flag = 0;
        }
        hhrtim1.Instance->sMasterRegs.MCMP1R = hhrtim1.Instance->sMasterRegs.MPER
                                        * LoopDutyOut;
}


void Inver1()
{
    static int temp;
    //
    //母线电压判定
    //
    if(InverterState == BusVoltageJudge)
    {
        ADC_Data.Vbus > 2.51f ? 
        temp++ : (temp=0);

        if(temp == 200)
            InverterState = GridConnectionJudge;
    }
    //
    //并网离网判定
    //
    else if(InverterState == GridConnectionJudge)
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
            lab5();//电压环
            LOW_C13;
        }
        
        else if(InverterMode == IntoGird)
        {
            if(SPLL_run())
            {
                if(SPLL_Mode.theta < 0.01f)
                    Contrarian_Bridge_Switch(ON);
                High_C13;
                lab7();//电流环
            }
            
        }
    }
}

void Inver2()
{
    static int temp;
    static float pastsin;
    //
    //母线电压判定
    //
    if(InverterState == BusVoltageJudge)
    {
        if(ADC_Data.Vbus > 2.51f)
            temp++ ;
        else
            temp=0;

        if(temp == 200)
        {
            InverterState = InverterRun;
        }
        LOW_C13;
    }

    //
    //锁相后开始跑环路
    //
    else if(InverterState == InverterRun)
    {
        if(SPLL_run())
        {
            if(pastsin < 0.0f && ADC_Data.Vgrid > 0.0f)
            {
                lab7();
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
                HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
                Contrarian_Bridge_Switch(ON);
                High_C13;
            }
            pastsin = ADC_Data.Vgrid;
        }
    }
}