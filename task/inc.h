#ifndef __INC_H__
#define __INC_H__

/*hal 模块*/
#include "gpio.h"
#include "main.h"
#include "spi.h"
#include "arm_math.h"
#include "hrtim.h"
#include "adc.h"
#include "dac.h"

/*基本模块*/
#include "mylib.h"
#include "OLEDUI.h"
#include "oled.h"
#include "keyboard.h"
#include "SEGGER_RTT.h"
#include "power_meas_sine_analyzer.h"
#include "spll_1ph_sogi.h"
#include "DCL.h"

/*标准库模块*/
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*常用算法模块*/
#include "SPWM.h"
#include "PID.h"
#include "PRcontroller.h"

void Task_Do();
void Task_Init();

#endif // !__INC_H__