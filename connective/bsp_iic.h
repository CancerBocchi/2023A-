/**
 * @file bsp_iic.h
 * @author your name (you@domain.com)
 * @brief 软件I2C 定义宏__SOFT_I2C__使用
 * @version 0.1
 * @date 2023-07-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _BSP_IIC_H
#define _BSP_IIC_H

#include "mylib.h"
#include "gpio.h"

#ifdef __SOFT_I2C__

#define u8        uint8_t 
#define u16       uint16_t
//IO方向设置
//#define SDA_IN()  {GPIOC->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO 
#define SCL_Port            SCL_GPIO_Port
#define SCL_PIN             SCL_Pin
#define SDA_Port            SDA_GPIO_Port
#define SDA_PIN             SDA_Pin    

#define GPIO_HIGH(Port,Pin) HAL_GPIO_WritePin(Port,Pin,SET)
#define GPIO_LOW(Port,Pin)  HAL_GPIO_WritePin(Port,Pin,RESET)
#define GPIO_Read(Port,Pin) HAL_GPIO_ReadPin(Port,Pin)

#define SCL_HIGH            GPIO_HIGH(SCL_Port,SCL_Pin)
#define SCL_LOW             GPIO_LOW(SCL_Port,SCL_Pin)
#define SDA_HIGH            GPIO_HIGH(SDA_Port,SDA_Pin)
#define SDA_LOW             GPIO_LOW(SDA_Port,SDA_Pin)

#define SDA_READ            GPIO_Read(SDA_Port,SDA_PIN)
#define SCL_READ            GPIO_Read(SCL_Port,SCL_PIN)


// #define GPIO_HIGH(Port,Pin)    
// #define GPIO_LOW(Port,Pin)     
// #define GPIO_Read(Port,Pin)    

// #define SCL_HIGH               SCL_Port->BSRR |= (0x01<<7)
// #define SCL_LOW                SCL_Port->BSRR |= (0x01<<23)
// #define SDA_HIGH               SDA_Port->BSRR |= (0x01<<6)
// #define SDA_LOW                SDA_Port->BSRR |= (0x01<<22)
// #define SDA_READ               (SDA_Port->IDR>>6)&0x01
// #define SCL_READ               (SCL_Port->IDR>>7)&0x01

//void mx_iic_init(void);                //��ʼ��IIC��IO��	
void SDA_IN();
void SDA_OUT();			 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

#endif
















