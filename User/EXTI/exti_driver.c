#include "exti_driver.h"

#if 0
void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	
	//初始化外部中断 EXTI0,EXTI1,EXTI2,EXTI3.
	EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line3;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	//中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling,  //下降沿触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
}
#else
void EXTI_Config(void)
{
	EXTI->IMR |= 0x0f;
	EXTI->FTSR |= 0x0f;
}

#endif

void EXTI_NVIC_InitConfig(void)
{
	NVIC_SetPriority(EXTI0_IRQn,0);
	NVIC_EnableIRQ(EXTI0_IRQn);
	
	NVIC_SetPriority(EXTI1_IRQn,1);
	NVIC_EnableIRQ(EXTI1_IRQn);
	
	NVIC_SetPriority(EXTI2_IRQn,2);
	NVIC_EnableIRQ(EXTI2_IRQn);
	
	NVIC_SetPriority(EXTI3_IRQn,3);
	NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI_InitConfig(void)
{
	EXTI_Config();
	EXTI_NVIC_InitConfig();
}

