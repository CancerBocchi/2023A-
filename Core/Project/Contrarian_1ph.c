#include "Contrarian_1ph.h"

DCL_PI             TI_VoltageCon;
DCL_PI             TI_CurrentCon;


Pos_PID_t          Voltage_Controller;
#if CurrentLoop == PI_
Pos_PID_t          Current_Controller;
#elif CurrentLoop == PR_
PR_t               Current_Controller;
#endif
Pos_PID_t           OpenVolt_Controller;

System_Flag               System_Flag_Data;
ADC_ValueTypeDef          ADC_Data;
POWER_MEAS_SINE_ANALYZER  Mea_Mode;//测量电参数
POWER_MEAS_SINE_ANALYZER  GridDetecter;//检测是否为电网模式
SPLL_1PH_SOGI             SPLL_Mode;
Inverter_Mode             InverterMode;
Inverter_Flag             InverterState;

float PR_Input;
float PR_theta;//电压期望变化的角度
float PR_Step;//单位步进
float PR_Rms;//存储电压环输出的值
float PR_VoltageRef_MAX;
float Keyboard_Input_Data[3];//0对应电压期望的幅值
float LoopDutyOut;//输出的占空比
float Max_Voltage;
float Max_Current;
float VrmsFliter;
float MD;
float Total_Iref;//总电流期望
float Device_Iref;//设备电流期望
float I_Proportion;//电流比例

uint8_t VoltLoop_Flag;
uint8_t CurrLoop_Flag;
uint8_t OVP_Flag;
uint8_t OIP_Flag;
uint8_t NIP_Flag;


void Contrarian_Init()
{
    //打开定时器 和 ADC
    HAL_HRTIM_WaveformCounterStart(&hhrtim1,HRTIM_TIMERID_MASTER);
    HAL_HRTIM_WaveformCounterStart(&hhrtim1,HRTIM_TIMERID_TIMER_A);
    HAL_HRTIM_WaveformCounterStart(&hhrtim1,HRTIM_TIMERID_TIMER_B);
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&ADC_Data.Raw_Value,4);
    HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);
    //键盘和屏幕的初始化
    Keyboard_Init();
    OLED_Init();
    //数据结构初始化
//有效电压环
#if (labrun == 1)||(labrun == 5)||(labrun == 6)

    Pos_PID_Init(&Voltage_Controller,0.01f,0.005f,0.0f);//0.01 0.001
    Voltage_Controller.Output_Max = 2.3f;
    Voltage_Controller.Output_Min = 0.0f;
    Voltage_Controller.Ref = 24.0f;
    Voltage_Controller.Value_I_Max = 5000.0f;

    POWER_MEAS_SINE_ANALYZER_reset(&Mea_Mode);
    POWER_MEAS_SINE_ANALYZER_config(&Mea_Mode,IT_FRE,0,AC_FRE+5,AC_FRE-5);

//瞬时电压环
#elif labrun == 2
    Pos_PID_Init(&Voltage_Controller,0.0f,0.008f,0.0f);//0.01 0.001
    Voltage_Controller.Output_Max = 3.2f;
    Voltage_Controller.Output_Min = -3.2f;
    Voltage_Controller.Ref = 0.0f;
    Voltage_Controller.Value_I_Max = 10000.0f; 

    PR_Rms = 16.0f;
#endif

#if (labrun == 1)||(labrun == 3)||(labrun == 5)||(labrun == 6)

    PR_Step = 2*PI*AC_FRE/CurLoop_FRE;

    Pos_PID_Init(&Current_Controller,0.01f,0.05f,0.0f);
    Current_Controller.Output_Max = 0.49f;
    Current_Controller.Output_Min = -0.49f;
    Current_Controller.Ref = 0.0f;
    Current_Controller.Value_I_Max = 10000.0f; 

#elif (labrun == 2)||(labrun == 4)
    PR_Step = 2*PI*AC_FRE/VolLoop_FRE;

    Pos_PID_Init(&Current_Controller,0.0f,0.01f,0.0f);
    Current_Controller.Output_Max = 0.49f;
    Current_Controller.Output_Min = -0.49f;
    Current_Controller.Ref = 0.0f;
    Current_Controller.Value_I_Max = 20000.0f; 

#endif

//开路电压环
#if (labrun == 5)||(labrun == 6)

    Pos_PID_Init(&OpenVolt_Controller,0.0f,0.0005f,0.0f);
    OpenVolt_Controller.Output_Max = 0.95f;
    OpenVolt_Controller.Output_Min = 0.05f;
    OpenVolt_Controller.Ref = 10.0f;
    OpenVolt_Controller.Value_I_Max = 5000.0f; 

#endif


#if Device == 1

    PR_Step = 2*PI*AC_FRE/CurLoop_FRE;

    Pos_PID_Init(&Current_Controller,-0.05f,-0.03f,0.0f);//0.01 0.006
    Current_Controller.Output_Max = 0.49f;
    Current_Controller.Output_Min = -0.49f;
    Current_Controller.Ref = 0.0f;
    Current_Controller.Value_I_Max = 1000.0f; 

    Pos_PID_Init(&OpenVolt_Controller,0.0f,0.0005f,0.0f);
    OpenVolt_Controller.Output_Max = 0.95f;
    OpenVolt_Controller.Output_Min = 0.05f;
    OpenVolt_Controller.Ref = 24.0f;
    OpenVolt_Controller.Value_I_Max = 10000.0f; 

#elif Device == 2
    PR_Step = 2*PI*AC_FRE/CurLoop_FRE;
    Pos_PID_Init(&Current_Controller,-0.05f,-0.03f,0.0f);//(0.007f)
    Current_Controller.Output_Max = 0.49f;
    Current_Controller.Output_Min = -0.49f;
    Current_Controller.Ref = 0.0f;
    Current_Controller.Value_I_Max = 1000.0f; 

    Pos_PID_Init(&OpenVolt_Controller,0.01f,0.0006f,0.0f);
    OpenVolt_Controller.Output_Max = 0.95f;
    OpenVolt_Controller.Output_Min = 0.05f;
    OpenVolt_Controller.Ref = 24.0f;
    OpenVolt_Controller.Value_I_Max = 1000.0f;  

#endif

    POWER_MEAS_SINE_ANALYZER_reset(&GridDetecter);
    POWER_MEAS_SINE_ANALYZER_config(&GridDetecter,IT_FRE,0,AC_FRE+5,AC_FRE-5);
    
    POWER_MEAS_SINE_ANALYZER_reset(&Mea_Mode);
    POWER_MEAS_SINE_ANALYZER_config(&Mea_Mode,IT_FRE,0,AC_FRE+5,AC_FRE-5);

    SPLL_1PH_SOGI_reset(&SPLL_Mode);
    SPLL_1PH_SOGI_config(&SPLL_Mode,AC_FRE,IT_FRE,SPLL_B0,SPLL_B1);

    ADC_Data.Iout = 0.0f;
    ADC_Data.Vout = 0.0f;
    ADC_Data.Raw_Value[0] = 0;
    ADC_Data.Raw_Value[1] = 0;

    Keyboard_Input_Data[1] = 1.0f;
    Keyboard_Input_Data[2] = 1.0f;

    Max_Voltage = VoltMax;
    Max_Current = CurrMax;

    System_Flag_Data.Contrarian_Bridge_State = OFF;
    System_Flag_Data.Contrarian_State = System_Stop;
    InverterState = GridConnectionJudge;
    InverterMode = InitValue;
    VoltLoop_Flag = 0;
    CurrLoop_Flag = 0;

}

void Contrarian_Bridge_Switch(bool OnOff)
{
    if(OnOff == ON)
    {
        if(!System_Flag_Data.Contrarian_Bridge_State)
        {
            System_Flag_Data.Contrarian_Bridge_State = ON;
            HAL_HRTIM_WaveformOutputStart(&hhrtim1,HRTIM_OUTPUT_TA1
                                                |HRTIM_OUTPUT_TA2
                                                |HRTIM_OUTPUT_TB1
                                                |HRTIM_OUTPUT_TB2);
        }
        return;
    }
    else if(OnOff == OFF)
    {
        if(System_Flag_Data.Contrarian_Bridge_State)
        {
            System_Flag_Data.Contrarian_Bridge_State = OFF;
            HAL_HRTIM_WaveformOutputStop(&hhrtim1,HRTIM_OUTPUT_TA1
                                                |HRTIM_OUTPUT_TA2
                                                |HRTIM_OUTPUT_TB1
                                                |HRTIM_OUTPUT_TB2);
        }
        return;
    }
}

void SystemStop()
{
    OIP_Flag = 0;
    OVP_Flag = 0;
    NIP_Flag = 0;
    VoltLoop_Flag = 0;
    CurrLoop_Flag = 0;
}

void ThetaUpdata()
{
    if(PR_theta > 2.0f*PI)
        PR_theta -= 2.0f*PI;
    else
        PR_theta += PR_Step;
}

bool SPLL_run()
{
    SPLL_1PH_SOGI_run(&SPLL_Mode,ADC_Data.Vgrid);
    if(SPLL_Mode.u_Q[0] < 0.01f)
        return true;
    else
        return false;
}

bool ModeDetect()
{
    static uint16_t flag;
    
    if(flag < (uint16_t)(Detect_Time*IT_FRE))
    {
        GridDetecter.v = ADC_Data.Vgrid;
        POWER_MEAS_SINE_ANALYZER_run(&GridDetecter);
        flag ++;
    }
    else if(flag == (uint16_t)(Detect_Time*IT_FRE))
    {
        if(GridDetecter.vRms >= 6.0f)
            InverterMode = IntoGird;
        else
            InverterMode = OutofGird;
        
        return true;
    }
    return false;
}

void Protect_Program()
{
    fabsf(ADC_Data.Vout) > 300.0f * SQRT_2?
        OVP_Flag++ : (OVP_Flag = 0);

    fabsf(ADC_Data.Iout) > 2.3f * SQRT_2 ?
        OIP_Flag++ : (OIP_Flag = 0);

    ADC_Data.Iout*ADC_Data.Vout < 0.0f ?
        NIP_Flag++ : (NIP_Flag = 0);

    if(OIP_Flag >= OIPStartNum)
    {
        System_Flag_Data.Contrarian_State = System_Fault;
        System_Flag_Data.SystemFaultState = Over_Curr;
        OIP_Flag = 0;
    }
    if(OVP_Flag >= OVPStartNum)
    {
        System_Flag_Data.Contrarian_State = System_Fault;
        System_Flag_Data.SystemFaultState = Over_Volt;
        OVP_Flag = 0;       
    }
    if(NIP_Flag >= 200)
    {
        System_Flag_Data.Contrarian_State = System_Fault;
        System_Flag_Data.SystemFaultState = Nega_Curr; 
        NIP_Flag = 0; 
    }

    if(System_Flag_Data.Contrarian_State == System_Fault)
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
    }
}
