#ifndef __KEYBOARD__
#define __KEYBOARD__

#include "gpio.h"

/**
 * @brief 
 * 采取行列扫描的方式
 * 默认扫描4*4的矩阵 如需更改,
 * 请更改.c中的for()和结构体数组的内容的数量
 * 还有键盘标号的计算公式（j*行数+列数+1）
 * 请把行命名为col_x 列命名为row_x
 * keyboard_scan()函数返回的是键盘标号
 * 使用if or switch来控制键盘的每一个按键
 * 行为input 列为output 且行下拉
 */

#define KeyboardStateChange (uint8_t)17

typedef enum 
{
    /* data */
    Integer_Input = 0x00,
    Decimal_Input = 0x01,
    Number_Generate = 0x02,
}InputMode_Typedef;

typedef enum
{
    Integer_NotBitsInput = 0x00,
    Integer_Input_FirBit = 0x01,
    Integer_Input_SecBit = 0x02,
}Integer_Flag;

typedef enum
{
    Decimal_NotBitsInput = 0x00,
    Decimal_Input_FirBit = 0x01,
    Decimal_Input_SecBit = 0x02,
}Decimal_Flag;

typedef struct 
{
    /* data */
    uint8_t Integer_Num[2];
    uint8_t Decimal_Num[2];
    uint8_t DataInput;
    uint8_t VariableNum;//变量序号
    float FinalNum;
    Integer_Flag Integer_State;
    Decimal_Flag Decimal_State;
    InputMode_Typedef InputState;
    
}Keyboard_Controll;


typedef struct KB
{
    GPIO_TypeDef* Port;
    uint32_t Pin;

}keyboard_pin;

extern Keyboard_Controll KeyBoardData;


void keyboard_execute(float *Data, float Num);
int keyboard_scan();
void Keyboard_Init();
void Keyboard_OLED(float *Data);

#endif