#ifndef __MYLIB__
#define __MYLIB__

#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "stdbool.h"
#include "main.h"
#include "usart.h"
//#include "tim.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define Euler 2.7182818284590452f
#define FACTOR_ADC 0.8056640625//adc转换成电压值的系数 单位为mv
#define FAIL -1

#define FLOAT_PRINTF(value) (int)(value*1000.0f)

#ifdef __TIM_H__
#define TIMER htim1
#endif

#define TogglePin_C13 HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13)
#define High_C13 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1)
#define LOW_C13 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0)

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


void Cancer_F_Str(char* buf, float target);
_Bool Cancer_JudgeKey_LTH(GPIO_TypeDef *Port,uint16_t Pin);
_Bool Cancer_JudgeKey_HTL(GPIO_TypeDef *Port,uint16_t Pin);;

#ifdef __ADC_H__

float Cancer_Get_ADC_Value(ADC_HandleTypeDef* hadc,uint32_t TIMEOUT);
void Cancer_ADC_Real(uint16_t*adc_value,float*real_value,uint16_t num);

#endif

#ifdef __USART_H__
void Block_UART_printf(UART_HandleTypeDef*uartbase,const char *format,...);
void DMA_UART_printf(UART_HandleTypeDef*uartbase,const char *format,...);
void IT_UART_printf(UART_HandleTypeDef*uartbase,const char *format,...);
#endif

#ifdef __TIM_H__
void Cancer_Delay_ms(uint32_t ms);
void Cancer_Delay_us(uint32_t us);
#endif

float Slew_Func(float *slewVal, float refVal, float slewRate);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif 
