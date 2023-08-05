#include "keyboard.h"
#include "gpio.h"
#include "inc.h"
#include "Contrarian_1ph.h"

int KeyBoard_Num;
Keyboard_Controll KeyBoardData;

keyboard_pin col[4]=
{
    {col_1_GPIO_Port,col_1_Pin},
    {col_2_GPIO_Port,col_2_Pin},
    {col_3_GPIO_Port,col_3_Pin},
    {col_4_GPIO_Port,col_4_Pin}
};

keyboard_pin row[4]=
{
    {row_1_GPIO_Port,row_1_Pin},
    {row_2_GPIO_Port,row_2_Pin},
    {row_3_GPIO_Port,row_3_Pin},
    {row_4_GPIO_Port,row_4_Pin},
};

int keyboard_scan()
{
    int temp=0;
    for(int i=0;i<4;i++)
    {
        HAL_GPIO_WritePin(col[i].Port,col[i].Pin,1);
        for(int j=0;j<4;j++)
        {
            if(HAL_GPIO_ReadPin(row[j].Port,row[j].Pin))
            {
                temp=j*4+i+1;
                //Block_UART_printf(&huart1,"%d,%d,%d\n",j,i,temp);
            }
        }
        HAL_GPIO_WritePin(col[i].Port,col[i].Pin,0);
    }
    return temp;
}

void Keyboard_Init()
{
    KeyBoardData.InputState = Integer_Input;
    KeyBoardData.Integer_State = Integer_NotBitsInput;
    KeyBoardData.Decimal_State = Decimal_NotBitsInput;

    KeyBoardData.DataInput = 18;
    KeyBoardData.Decimal_Num[0]= 0;
    KeyBoardData.Decimal_Num[1]= 0;
    KeyBoardData.FinalNum = 0.0f;
    KeyBoardData.Integer_Num[0] = 0;
    KeyBoardData.Integer_Num[1] = 0;
    KeyBoardData.VariableNum = 0;
}

void Keyboard_Reset()
{
    KeyBoardData.InputState = Integer_Input;
    KeyBoardData.Integer_State = Integer_NotBitsInput;
    KeyBoardData.Decimal_State = Decimal_NotBitsInput;

    KeyBoardData.DataInput = 18;
    KeyBoardData.Decimal_Num[0]= 0;
    KeyBoardData.Decimal_Num[1]= 0;
    KeyBoardData.FinalNum = 0.0f;
    KeyBoardData.Integer_Num[0] = 0;
    KeyBoardData.Integer_Num[1] = 0;
}

/**
 * @brief 执行函数 switch语句中填写需要的内容
 * 
 */
void keyboard_execute(float *Data, float Num)
{
    static int KeyBoard_Tick = 0;
    KeyBoard_Num = keyboard_scan();
    if(KeyBoard_Num)
    {
        SEGGER_RTT_printf(0,"%d\n",KeyBoard_Num);
        if(HAL_GetTick() - KeyBoard_Tick >= 200)
        {
            if(KeyBoard_Num <= 3 && KeyBoard_Num >= 1)
                KeyBoardData.DataInput = KeyBoard_Num;

            else if(KeyBoard_Num <= 7 && KeyBoard_Num >= 5)
                KeyBoardData.DataInput = KeyBoard_Num - 1;
            
            else if(KeyBoard_Num <= 11 && KeyBoard_Num >= 9)
                KeyBoardData.DataInput = KeyBoard_Num - 2;

            else if(KeyBoard_Num == 14)
                KeyBoardData.DataInput = 0;
            
            else if(KeyBoard_Num == 13)
                KeyBoardData.DataInput = KeyboardStateChange;

            else if(KeyBoard_Num == 15)
                KeyBoardData.InputState = Number_Generate;

            else if(KeyBoard_Num == 8)
                KeyBoardData.VariableNum < Num-1 ? KeyBoardData.VariableNum++ : (KeyBoardData.VariableNum = 0);

            else if(KeyBoard_Num == 4)   
                KeyBoardData.VariableNum > 0 ? KeyBoardData.VariableNum-- : (KeyBoardData.VariableNum = Num-1);
            
            else if(KeyBoard_Num ==12)
            {
                if(KeyBoardData.InputState == Integer_Input)
                {
                    if(KeyBoardData.Integer_State == Integer_Input_FirBit)
                    {
                        KeyBoardData.Integer_Num[0] = 0;
                        KeyBoardData.Integer_State = Integer_NotBitsInput;
                    }
                    else if(KeyBoardData.Integer_State == Integer_Input_SecBit)
                    {
                        KeyBoardData.Integer_Num[1] = 0;
                        KeyBoardData.Integer_State = Integer_Input_FirBit;
                    }
                }
                else if(KeyBoardData.InputState == Decimal_Input)
                {
                    if(KeyBoardData.Decimal_State == Decimal_Input_FirBit)
                    {
                        KeyBoardData.Decimal_Num[0] = 0;
                        KeyBoardData.Decimal_State = Decimal_NotBitsInput;
                    }
                    else if(KeyBoardData.Decimal_State == Decimal_Input_SecBit)
                    {
                        KeyBoardData.Decimal_Num[1] = 0;
                        KeyBoardData.Decimal_State = Decimal_Input_FirBit;
                    }
                    else if(KeyBoardData.Decimal_State == Decimal_NotBitsInput)
                    {
                        KeyBoardData.InputState = Integer_Input;                       
                    }
                }
            }

            else if (KeyBoard_Num == 16)
            {
                if(System_Flag_Data.Contrarian_State == System_Run)
                {
                    System_Flag_Data.Contrarian_State = System_Stop;
                }
                else
                {
                    System_Flag_Data.SystemFaultState = NoFault;
                    System_Flag_Data.Contrarian_State = System_Run;
                }
            }


            switch (KeyBoardData.InputState)
            {
            case Integer_Input:
                if(KeyBoardData.DataInput <=9 && KeyBoardData.DataInput >= 0)
                {
                    if(KeyBoardData.Integer_State == Integer_NotBitsInput)
                    {
                        KeyBoardData.Integer_State = Integer_Input_FirBit;
                        KeyBoardData.Integer_Num[0] = KeyBoardData.DataInput;
                    }
                    else if(KeyBoardData.Integer_State == Integer_Input_FirBit)
                    {
                        KeyBoardData.Integer_State = Integer_Input_SecBit;
                        KeyBoardData.Integer_Num[1] = KeyBoardData.DataInput;
                    }
                }
                else if(KeyBoardData.DataInput == KeyboardStateChange)
                {
                    KeyBoardData.InputState = Decimal_Input;
                }
                break;
            
            case Decimal_Input:
                if(KeyBoardData.DataInput <=9 && KeyBoardData.DataInput >= 0)
                {
                    if(KeyBoardData.Decimal_State == Decimal_NotBitsInput)
                    {
                        KeyBoardData.Decimal_State = Decimal_Input_FirBit;
                        KeyBoardData.Decimal_Num[0] = KeyBoardData.DataInput;
                    }
                    else if(KeyBoardData.Decimal_State == Decimal_Input_FirBit)
                    {
                        KeyBoardData.Decimal_State = Decimal_Input_SecBit;
                        KeyBoardData.Decimal_Num[1] = KeyBoardData.DataInput;
                    }
                }
                break;

            case Number_Generate:
                if(!(KeyBoardData.Decimal_State||KeyBoardData.Integer_State))//如果两者都没有输入
                    KeyBoardData.InputState = Integer_Input;

                else if(KeyBoardData.Integer_State == Integer_NotBitsInput)
                    KeyBoardData.FinalNum = 0.1f*(float)KeyBoardData.Decimal_Num[0] + 0.01f* (float)KeyBoardData.Decimal_Num[1];               
                
                else if(KeyBoardData.Integer_State == Integer_Input_FirBit)
                    KeyBoardData.FinalNum = 0.1f*(float)KeyBoardData.Decimal_Num[0] + 0.01f* (float)KeyBoardData.Decimal_Num[1]
                                            + KeyBoardData.Integer_Num[0];
                
                else if(KeyBoardData.Integer_State == Integer_Input_SecBit)
                    KeyBoardData.FinalNum = 0.1f*(float)KeyBoardData.Decimal_Num[0] + 0.01f* (float)KeyBoardData.Decimal_Num[1]
                                            + 10 * KeyBoardData.Integer_Num[0] + KeyBoardData.Integer_Num[1];
                
                Data[KeyBoardData.VariableNum] = KeyBoardData.FinalNum;

                Keyboard_Reset();
                
                break;
            }

            KeyBoardData.DataInput = 18;
            KeyBoard_Tick = HAL_GetTick();
            KeyBoard_Num = 0;
        }
    }
}

void Keyboard_OLED(float *Data)
{
    OLED_CleanBuffer();

    DrawRoundRect(2,1,126,60,5);
    DrawRoundRect(6,5,41,9,2);
    DrawLine(90,1,90,61);
    DrawLine(91,11,128,11);
    if(KeyBoardData.Integer_State == Integer_Input_SecBit)
    {
        DrawNum(8,6,KeyBoardData.Integer_Num[0],1,1,0);
        DrawNum(16,6,KeyBoardData.Integer_Num[1],1,1,0);
    }
    else
    {
        DrawNum(8,6,KeyBoardData.Integer_Num[1],1,1,0);
        DrawNum(16,6,KeyBoardData.Integer_Num[0],1,1,0);
    }
    DrawChar(24,6,'.',1,0);
    DrawNum(32,6,KeyBoardData.Decimal_Num[0],1,1,0);
    DrawNum(40,6,KeyBoardData.Decimal_Num[1],1,1,0);

    if(KeyBoardData.VariableNum == 0)
    {
        DrawString(6,22,"Vref:",1,0);
        DrawString(6,34,"Iref:",1,1);
        DrawString(6,46,"Data3:",1,1);
    }
    if(KeyBoardData.VariableNum == 1)
    {
        DrawString(6,22,"Vref:",1,1);
        DrawString(6,34,"Iref:",1,0);
        DrawString(6,46,"Data3:",1,1);
    }
    if(KeyBoardData.VariableNum == 2)
    {
        DrawString(6,22,"Vref:",1,1);
        DrawString(6,34,"Iref:",1,1);
        DrawString(6,46,"Data3:",1,0); 
    }

    OLED_DrawFloat(42,22,Data[0]);
    OLED_DrawFloat(42,34,Data[1]);
    OLED_DrawFloat(42,46,Data[2]);

    switch(System_Flag_Data.Contrarian_State)
    {
    case System_Run:
        DrawString(92,3,"*Run*",1,1);
        break;

    case System_Stop:
        DrawString(92,3,"*Stop",1,1);
        break;

    case System_Fault:
        if(System_Flag_Data.SystemFaultState == Over_Curr)
            DrawString(92,3,"*OIP*",1,1);
        else if(System_Flag_Data.SystemFaultState == Over_Volt)
            DrawString(92,3,"*OVP*",1,1);
        else if(System_Flag_Data.SystemFaultState == Nega_Curr)
            DrawString(92,3,"*NIP*",1,1);
        break;
    default:

        break;
    }

    OLED_DrawFloat(92,15,Current_Controller.Error);
    OLED_DrawFloat(92,23,PR_Rms);
    OLED_DrawFloat(92,31,Voltage_Controller.Error);

#if Device == 1
    if(InverterMode == OutofGird)
        DrawString(92,52,"OGRID",1,1);
    else if(InverterMode == IntoGird)
        DrawString(92,52,"IGRID",1,1);
    else if(InverterMode == InitValue)
        DrawString(92,52,"INIT",1,1);
#endif 
    flash();

}