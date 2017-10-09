#include "led_driver.h"

/**********************************************
** 3色LED初始化
**********************************************/
void Led_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//APB2时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//初始化结构体
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	//初始化-关闭LED灯
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
}
/**********************************************
** 3色LED控制
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
