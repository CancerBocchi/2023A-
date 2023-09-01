#ifndef __CONTRARIAN_1PH_H__
#define __CONTRARIAN_1PH_H__

#include "Contrarian_1ph_ADC.h"
#include "Contrarian_Defines.h"
#include "inc.h"

typedef enum
{
    System_Stop  = 0x00,
    System_Run   = 0x01,
    System_Fault = 0x02,
    System_Init  = 0x03,
}System_State;

typedef enum
{
    NoFault   = 0x00,
    Over_Volt = 0x01,
    Over_Curr = 0x02,
    Nega_Curr = 0x03,
}Fault_Typedef;

typedef struct 
{
    /* data */
    bool Contrarian_Bridge_State;
    System_State Contrarian_State;
    Fault_Typedef SystemFaultState;

}System_Flag;

typedef enum
{
    InitValue = 0x00,
    IntoGird  = 0x01,
    OutofGird = 0x02,
}Inverter_Mode;

typedef enum
{
    GridConnectionJudge = 0x00,
    SpllLockedJudge     = 0x01,
    InverterRun         = 0x02,
}Inverter_Flag;

extern Pos_PID_t          Voltage_Controller;
#if CurrentLoop == PI_
extern Pos_PID_t          Current_Controller;
#elif CurrentLoop == PR_
extern PR_t               Current_Controller;
#endif
extern Pos_PID_t                    OpenVolt_Controller;
extern System_Flag                  System_Flag_Data;
extern ADC_ValueTypeDef             ADC_Data;
extern POWER_MEAS_SINE_ANALYZER     Mea_Mode;
extern DCL_PI                       TI_VoltageCon;
extern DCL_PI                       TI_CurrentCon;
extern POWER_MEAS_SINE_ANALYZER     GridDetecter;//检测是否为电网模式
extern SPLL_1PH_SOGI                SPLL_Mode;
extern Inverter_Mode                InverterMode;
extern Inverter_Flag                InverterState;

extern float PR_Input;
extern float PR_theta;//电压期望变化的角度
extern float PR_Step;//单位步进
extern float PR_Rms;//存储电压环输出的值
extern float PR_VoltageRef_MAX;//电压幅值
extern float Keyboard_Input_Data[3];//0对应电压期望的幅值
extern float LoopDutyOut;
extern float VrmsFliter;
extern float MD;
extern float Total_Iref;
extern float Device_Iref;//设备电流期望
extern float I_Proportion;//电流比例

extern uint8_t VoltLoop_Flag;
extern uint8_t CurrLoop_Flag;
extern uint8_t OVP_Flag;
extern uint8_t OIP_Flag;
extern uint8_t NIP_Flag;

bool SPLL_run();
bool ModeDetect();
void SystemStop();
void ThetaUpdata();
void Contrarian_Init();
void Protect_Program();
void Contrarian_Bridge_Switch(bool OnOff);


void lab1();
void lab2();
void lab3();
void lab4();
void lab5();
void lab6();
void lab7();
void lab8();

void Inver1();
void Inver2();

#endif