#include "key_driver.h"
#include "SysTick.h"
#include "led_driver.h"

/*********************************************
**��ʼ������
*********************************************/
void Key_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//��ʼ��PA0 1 2 3��Ϊ�������������������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*********************************************
**��ʱ
*********************************************/
void Key_delay(void)
{
	delay_ms(20);
}

/*********************************************
**�ж��ĸ�����������
*********************************************/
__KEY_TYPE Get_KeyType(void)
{
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))	//����������
	{
		Key_delay();	//����
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));//�������ɿ�
			return MSG_KEY0;
		}	
	}

	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
	{
		Key_delay();
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
			return MSG_KEY1;
		}	
	}
	
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
	{
		Key_delay();
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2));
			return MSG_KEY2;
		}	
	}
	
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
	{
		Key_delay();
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));
			return MSG_KEY3;
		}	
	}
	return NO_KEY;
}

/*********************************************
**����ɨ��
*********************************************/
void Key_Scan(__KEY_TYPE key_type)
{
	static int i = 0;
	switch((uint8_t)key_type)
	{
		case MSG_KEY0:
			Led_Ctl(LED_RED,ON);
		break;
		case MSG_KEY1:
			Led_Ctl(LED_GREEN,ON);
		break;
		case MSG_KEY2:
			Led_Ctl(LED_BLUE,ON);
		break;
		case MSG_KEY3:
			if(!(i%3))
			{
				Led_Ctl(LED_RED,OFF);
			}
			if(i%3 == 1)
			{
				Led_Ctl(LED_GREEN,OFF);
			}
			if(i%3 == 2)
			{
				Led_Ctl(LED_BLUE,OFF);
			}
			i++;
		break;
	}
	

}







