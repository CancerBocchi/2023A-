#include "main.h"


#define OLED_CS_CLR HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, RESET)
#define OLED_CS_SET  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, SET)

#define OLED_DC_CLR  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, RESET) //低为命令
#define OLED_DC_SET  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin ,SET)   //高为数据

#define OLED_RST_CLR  HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, RESET)
#define OLED_RST_SET  HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, SET)


void OLED_Reset(void);
void OLED_Init(void);
void OLED_Clear(void);

void OLED_WriteAdd(uint8_t);
void OLED_WriteData(uint8_t);


/*
// void OLED_Show_Number_16(uint8_t x,uint8_t  y,uint8_t  number );
// void OLED_Show_Zimu_16(uint8_t x,uint8_t  y,uint8_t  number );
// void OLED_Show_Number_40x32(uint8_t x,uint8_t  y,uint8_t  number );
*/
