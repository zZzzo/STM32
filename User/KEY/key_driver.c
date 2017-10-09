#include "key_driver.h"
#include "SysTick.h"
#include "led_driver.h"

/*********************************************
**初始化按键
*********************************************/
void Key_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//初始化PA0 1 2 3口为浮空输入或者上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*********************************************
**延时
*********************************************/
void Key_delay(void)
{
	delay_ms(20);
}

/*********************************************
**判断哪个按键被按下
*********************************************/
__KEY_TYPE Get_KeyType(void)
{
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))	//按键被按下
	{
		Key_delay();	//消抖
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));//按键被松开
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
**按键扫描
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







