#include "i2c_driver.h"
#include "systick.h"
#include <stdio.h>

#define GPIO_PORT_I2C	GPIOB	/*IIC��GPIO�˿�*/
#define RCC_I2C_PORT	RCC_APB2Periph_GPIOB	/*IIC��GPIO�˿ڵ�ʱ��*/
#define I2C_SCL_PIN		GPIO_Pin_6		/*����SCLʱ���ߵ�GPIO��*/
#define I2C_SDA_PIN		GPIO_Pin_7		/*����SDA�����ߵ�GPIO��*/

#if 0	/*GPIO�Ŀ⺯��ʵ��IO��д*/
	#define I2C_SCL_1()		GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)
	#define I2C_SCL_0()		GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)

	#define I2C_SDA_1()		GPIO_SetBits(GPIO_PORT_I2C,I2C_SDA_PIN)
	#define I2C_SDA_0()		GPIO_ResetBits(GPIO_PORT_I2C,I2C_SDA_PIN)
	
	#define I2C_SDA_READ()		GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)
#else	/*�����Ĵ���ʵ��IO��д*/
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
	
	//I2C�����ڿ���ʱ�����ߵĵ�ƽ״̬Ϊ�ߵ�ƽ
	GPIO_SetBits(GPIO_PORT_I2C,I2C_SCL_PIN|I2C_SDA_PIN);
}

//I2C�����ߣ�ʱ������Ϊ����ģ�SDA������״̬��

/**SDA_OUT**/
static void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
//	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStruct);
	
	//I2C�����ڿ���ʱ�����ߵĵ�ƽ״̬Ϊ�ߵ�ƽ
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
**��������:��ʼ�ź�
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
**��������:��ֹ�ź�
**************************************/
void I2C_Stop(void)
{
	SDA_OUT();
	I2C_SDA_0();
	I2C_SCL_1();
	delay_us(8);
	I2C_SDA_1();
	delay_us(8);
	//��ֹ�˾Ͳ����ٽ�SCL������
}
/**************************************
**��������:����Ӧ���ź�
**��	��:ack----0---->ACK �϶���Ӧ��
**		      ----1---->NACK �񶨵�Ӧ��
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
**��������:����Ӧ���ź�
**����	ֵ:0-----ACK
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
**��������:����1Byte����
**��	��:dat-----Ҫ���͵�����
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
**��������:��ȡ1Byte����
**����	ֵ:��ȡ��������
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
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
****************************************************************************/
u8 I2C_CheckDevice(u8 _Address)
{
	u8 ucAck;
	//��ʼ�ź�
	I2C_Start();
	//����I2C�豸��ַ + ��д����λ 
	I2C_SendByte(_Address | I2C_WR);
	//��ȡ���ص���Ӧ�ź�
	ucAck = I2C_RecvACK();	
	//��ֹ�ź�
	I2C_Stop();
	
	return ucAck;
}






