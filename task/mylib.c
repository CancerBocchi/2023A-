/*在cube中需要打开adc usart 
*本文件才能正常使用
*否则手动进入mylib.h中屏蔽相应头文件包含
*/

#include"mylib.h"

/*
* @brief  : 检测IO口变化函数 LTH默认低电平 HTL默认高电平
* @param  : Port IO端口 Pin 引脚标号
* @retval : true为变化 false为不变
*/

float Slew_Func(float *slewVal, float refVal, float slewRate)
{
  static float diff = 0;
  diff = refVal - *slewVal;
  if (diff >= slewRate)
  {
    *slewVal += slewRate;
    return (1);
  } else if (-diff >= slewRate)
  {
    *slewVal -= slewRate;
    return (-1);
  } else
  {
    *slewVal = refVal;
    return (0);
  }
}

_Bool Cancer_JudgeKey_LTH(GPIO_TypeDef *Port,uint16_t Pin)
{
	static uint32_t tick;
	static uint8_t flag = 1;
	if(flag)
	{
		tick = HAL_GetTick();
		flag = 0;
	}
	if(HAL_GPIO_ReadPin(Port,Pin))
	{
		if(HAL_GetTick() - tick > 1000)
		{
			return true;
		}
		tick = HAL_GetTick();
	}
	return false;
}

_Bool Cancer_JudgeKey_HTL(GPIO_TypeDef *Port,uint16_t Pin)
{
	static uint32_t tick;
	static uint8_t flag = 1;
	if(flag)
	{
		tick = HAL_GetTick();
		flag = 0;
	}
	if(!HAL_GPIO_ReadPin(Port,Pin))
	{
		if(HAL_GetTick() - tick > 100)
		{
			return true;
		}
		tick = HAL_GetTick();
	}
	return false;
}

void Cancer_F_Str(char* buf, float target)
{
	int digit = (int)target;
	int frac = (int)((target - (float)digit) * 1000000);
	if (frac < 10)
		sprintf(buf, "%d.00000%d\n", digit, frac);
	else if (frac < 100 && frac >= 10)
		sprintf(buf, "%d.0000%d\n", digit, frac);
	else if(frac<1000&&frac>=100)
		sprintf(buf, "%d.000%d\n", digit, frac);
	else if(frac<10000&&frac>=1000)
		sprintf(buf, "%d.00%d\n", digit, frac);
	else if(frac<100000&&frac>=10000)
		sprintf(buf, "%d.0%d\n", digit, frac);
	else if(frac<1000000&&frac>=100000)
		sprintf(buf, "%d.%d\n", digit, frac);
}

#ifdef __ADC_H__
/*for：adc查询模式
* why：封装函数
* variable:hadc为对应adc的地址 timeout为超时时间
* 若转换完成返回adc的值反之返回-1
*/
float Cancer_Get_ADC_Value(ADC_HandleTypeDef* hadc,uint32_t TIMEOUT)
{
	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc,TIMEOUT);
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(hadc),HAL_ADC_STATE_REG_EOC))//判断是否以及完成转换
	{
		return HAL_ADC_GetValue(hadc)*FACTOR_ADC;
	}
	else
	return FAIL;
}
/*for:将adc的值自动转换为value
*charactor：DMA模式时候的转换
*variable:依次为adc值，真实值，数组元素个数 前两者数组元素个数尽量保持一致
*ps:提前设置好dma并打开
*/
void Cancer_ADC_Real(uint16_t*adc_value,float*real_value,uint16_t num)
{
	for(int i=0;i<num;i++)
	{
		real_value[i]=adc_value[i]*FACTOR_ADC;
	}
}

#endif

#ifdef __USART_H__
/*
* @brief:串口打印函数
* @param：串口编号
* @param: 输出字符串
*/
void Block_UART_printf(UART_HandleTypeDef*uartbase,const char *format,...)
{
	va_list ap;
	va_start(ap,format);
	int length;
	char buffer[128];
	length=vsnprintf(buffer,128,format,ap);
	HAL_UART_Transmit(uartbase,(uint8_t*)buffer,length,0xffff);
}

void DMA_UART_printf(UART_HandleTypeDef*uartbase,const char *format,...)
{
	va_list ap;
	va_start(ap,format);
	int length;
	char buffer[128];
	length=vsnprintf(buffer,128,format,ap);
	HAL_UART_Transmit_DMA(uartbase,(uint8_t*)buffer,length);
}

void IT_UART_printf(UART_HandleTypeDef*uartbase,const char *format,...)
{
	va_list ap;
	va_start(ap,format);
	int length;
	char buffer[128];
	length=vsnprintf(buffer,128,format,ap);
	HAL_UART_Transmit_IT(uartbase,(uint8_t*)buffer,length);
}

#endif

#ifdef __TIM_H__

void Cancer_Delay_us(uint32_t us)
{
	__HAL_TIM_ENABLE(&TIMER);
	int timebase = __HAL_TIM_GET_COUNTER(&TIMER);
	while(__HAL_TIM_GetCounter(&TIMER)-timebase<us)
	{}
	__HAL_TIM_DISABLE(&TIMER);
	
}

void Cancer_Delay_ms(uint32_t ms)
{
	for(int i =0;i<ms;i++)
	{
		Cancer_Delay_us(1000);
	}
}

#endif
