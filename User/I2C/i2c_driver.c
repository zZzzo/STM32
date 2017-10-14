#include "i2c_driver.h"
#include "systick.h"
#include <stdio.h>

#define GPIO_PORT_I2C	GPIOB	/*IIC的GPIO端口*/
#define RCC_I2C_PORT	RCC_APB2Periph_GPIOB	/*IIC的GPIO端口的时钟*/
#define I2C_SCL_PIN		GPIO_Pin_6		/*连接SCL时钟线的GPIO口*/
#define I2C_SDA_PIN		GPIO_Pin_7		/*连接SDA数据线的GPIO口*/

#if 0	/*GPIO的库函数实现IO读写*/
	#define I2C_SCL_1()		GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)
	#define I2C_SCL_0()		GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)

	#define I2C_SDA_1()		GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN)
	#define I2C_SDA_0()		GPIO_ResetBits(GPIO_PORT_I2C,I2C_SDA_PIN)
	
	#define I2C_SDA_READ()		GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)
#else	/*操作寄存器实现IO读写*/
	#define I2C_SCL_1()		GPIO_PORT_I2C->BSRR = I2C_SCL_PIN	/* SCL = 1*/
	#define I2C_SCL_0()		GPIO_PORT_I2C->BRR = I2C_SCL_PIN	/* SCL = 0*/	
	
	#define I2C_SDA_1()		GPIO_PORT_I2C->BSRR = I2C_SDA_PIN	/* SDA = 1*/
	#define I2C_SDA_0()		GPIO_PORT_I2C->BRR = I2C_SDA_PIN	/* SDA = 0*/
	
	#define I2C_SDA_READ()	((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)
#endif

void I2C_GPIOInitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = I2C_SCL_PIN|I2C_SDA_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStruct);
	
	//I2C总线在空闲时，总线的电平状态为高电平
	GPIO_SetBits(GPIO_PORT_I2C,I2C_SCL_PIN|I2C_SDA_PIN);
}

//I2C两条线，时钟线总为输出的，SDA有两种状态。

/**SDA_OUT**/
static void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
//	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStruct);
	
	//I2C总线在空闲时，总线的电平状态为高电平
	GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN);
}

/**SDA_IN**/
static void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN);
}

/**************************************
**函数功能:起始信号
**************************************/
void I2C_Start(void)
{
	SDA_OUT();
	I2C_SDA_1();
	I2C_SCL_1();
	delay_us(8);
	I2C_SDA_0();
	delay_us(8);
	I2C_SCL_0();
}
/**************************************
**函数功能:终止信号
**************************************/
void I2C_Stop(void)
{
	SDA_OUT();
	I2C_SDA_0();
	I2C_SCL_1();
	delay_us(8);
	I2C_SDA_1();
	delay_us(8);
	//终止了就不用再将SCL拉低了
}
/**************************************
**函数功能:发送应答信号
**参	数:ack----0---->ACK 肯定的应答
**		      ----1---->NACK 否定的应答
**************************************/
void I2C_SendACK(u8 ask)
{
	SDA_OUT();
	I2C_SCL_0();
	delay_us(8);
	if(ask)
		I2C_SDA_1();
	else
		I2C_SDA_0();
	I2C_SCL_1();
	delay_us(8);
	I2C_SCL_0();
	delay_us(8);
}
/**************************************
**函数功能:接收应答信号
**返回	值:0-----ACK
**		   1-----NACK
**************************************/
u8 I2C_RecvACK(void)
{
	u8 ucErrTime = 0;
	SDA_IN();
	I2C_SDA_1();
	delay_us(4);
	I2C_SCL_1();
	delay_us(4);
	while(I2C_SDA_READ())
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			I2C_Stop();
			return 1;
		}
	}		
	I2C_SCL_0();
	return 0;
}


/**************************************
**函数功能:发送1Byte数据
**参	数:dat-----要发送的数据
**************************************/
void I2C_SendByte(u8 dat)
{
	u8 t; 
	SDA_OUT(); 	    
    I2C_SCL_0();
    for(t=0; t<8; t++)
    {
		if(dat&0x80)
			I2C_SDA_1();
		else I2C_SDA_0();
        dat<<=1; 	  
		delay_us(5);
		I2C_SCL_1();
		delay_us(5); 
		I2C_SCL_0();
		delay_us(5);
    }
}

/**************************************
**函数功能:读取1Byte数据
**返回	值:读取到的数据
**************************************/	
u8 I2C_RecvByte(void)
{
	u8 i = 0;
	u8 dat = 0;
	SDA_IN();
	for(i=0; i<8; i++)
	{
		delay_us(5);
		I2C_SCL_1();
		delay_us(5);
		dat <<= 1;
		if(I2C_SDA_READ())
			dat |= 0x01;
		I2C_SCL_0();
		delay_us(5);
	}
	return dat;
}
/****************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
****************************************************************************/
u8 I2C_CheckDevice(u8 _Address)
{
	u8 ucAck;
	//起始信号
	I2C_Start();
	//发送I2C设备地址 + 读写控制位 
	I2C_SendByte(_Address | I2C_WR);
	//读取返回的响应信号
	ucAck = I2C_RecvACK();	
	//终止信号
	I2C_Stop();
	
	return ucAck;
}






