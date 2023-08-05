#include "bsp_iic.h"
#ifdef __SOFT_I2C__
//初始化IIC
//void mx_iic_init(void)
// {					     

// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
// }

void SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStruct;
 	GPIO_InitStruct.Pin = SDA_Pin;                    //使用STM3cubemx是定义好的SDA_Pin为GPIO_PIN_8的标签
  	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  	HAL_GPIO_Init(SDA_Port, &GPIO_InitStruct);
}

void SDA_IN()
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = SDA_Pin;                    //使用STM3cubemx是定义好的SDA_Pin为GPIO_PIN_8的标签
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SDA_Port, &GPIO_InitStruct);

}

//产生IIC起始信号
void IIC_Start(void)
{
	//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	SDA_OUT();     //sda线输出
	//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	// IIC_SDA=1;	  	  
	// IIC_SCL=1;
	SDA_HIGH;
	SCL_HIGH;
	//delay_us(4);
 	// IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	SDA_LOW;
	//delay_us(4);
	// IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	SCL_LOW;
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	SDA_OUT();//sda线输出
	//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	// IIC_SCL=0;
	// IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	SCL_LOW;
	SDA_LOW;
 	//delay_us(4);
	// IIC_SCL=1; 
	// IIC_SDA=1;//发送I2C总线结束信号
	SCL_HIGH;
	SDA_HIGH;
	// delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	//IIC_SDA=1;
	SDA_HIGH;
	//delay_us(1);	   
	// IIC_SCL=1;
	SCL_HIGH;
	//delay_us(1);	 
	while(SDA_READ)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	// IIC_SCL=0;//时钟输出0 
	SCL_LOW;	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	// IIC_SCL=0;
	SCL_LOW;
	SDA_OUT();
	// IIC_SDA=0;
	SDA_LOW;
	//delay_us(4);
	// IIC_SCL=1;
	SCL_HIGH;
	//delay_us(4);
	// IIC_SCL=0;
	SCL_LOW;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	// IIC_SCL=0;
	SCL_LOW;
	SDA_OUT();
	// IIC_SDA=1;
	SDA_HIGH;
	//delay_us(2);
	// IIC_SCL=1;
	SCL_HIGH;
	//delay_us(2);
	// IIC_SCL=0;
	SCL_LOW;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    //IIC_SCL=0;//拉低时钟开始数据传输
	SCL_LOW;
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			//IIC_SDA=1;
			SDA_HIGH;
		else
			//IIC_SDA=0;
			SDA_LOW;
		txd<<=1; 	  
		//delay_us(2);   //对TEA5767这三个延时都是必须的
		//IIC_SCL=1;
		SCL_HIGH;
		//delay_us(2); 
		//IIC_SCL=0;
		SCL_LOW;
		//delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        //IIC_SCL=0; 
		SCL_LOW;
        //delay_us(2);
		//IIC_SCL=1;
		SCL_HIGH;
        receive<<=1;
        if(SDA_READ)receive++;   
		//delay_us(2); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

// void SCL_HIGH()
// {
// 	SCL_Port->BSRR |= 0x40; 
// }

// void SCL_LOW()
// {
// 	SCL_Port->BSRR &= 0x7FFFFFBF;
// }

// void SDA_HIGH()
// {
// 	SDA_Port->BSRR |= 0x20;
// }

// void SDA_LOW()
// {
// 	SDA_Port->BSRR &= 0x7FFFFFDF;
// }
#endif