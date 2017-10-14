#include "tim_driver.h"

void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;//预分频系数7199
	TIM_TimeBaseInitStruct.TIM_Period = 9999;	//定时1s的计数 计数频率10KHz，计数1次要用100us，
												//1s就计数了10*10^3次。
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);//开启TIM2中断
	//计数完成，溢出，更新事件，产生中断。
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//清空事件更新标志位
	//使能TIM2
	TIM_Cmd(TIM2,ENABLE);
}

void TIM_NVIC_InitConfig(void)
{
	NVIC_SetPriority(TIM2_IRQn,2);
	NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM_InitConfig(void)
{
	TIM_Config();
	TIM_NVIC_InitConfig();
}


