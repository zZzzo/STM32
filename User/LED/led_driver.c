#include "led_driver.h"

/**********************************************
** 3ɫLED��ʼ��
**********************************************/
void Led_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//APB2ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//��ʼ���ṹ��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	//��ʼ��-�ر�LED��
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
}
/**********************************************
** 3ɫLED����
**********************************************/
void Led_Ctl(__LED_TYPE type,uint8_t statue)
{
	switch((uint8_t)type)
	{
		case LED_RED:
			if(statue)
				GPIO_SetBits(GPIOC, GPIO_Pin_8);
			else	
				GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		break;
		case LED_GREEN:
			if(statue)
				GPIO_SetBits(GPIOC, GPIO_Pin_7);
			else
				GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		break;
		case LED_BLUE:
			if(statue)
				GPIO_SetBits(GPIOC, GPIO_Pin_6);
			else
				GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		break;
	}
}
