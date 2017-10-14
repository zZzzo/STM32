#include "tim_driver.h"

void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;//Ԥ��Ƶϵ��7199
	TIM_TimeBaseInitStruct.TIM_Period = 9999;	//��ʱ1s�ļ��� ����Ƶ��10KHz������1��Ҫ��100us��
												//1s�ͼ�����10*10^3�Ρ�
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);//����TIM2�ж�
	//������ɣ�����������¼��������жϡ�
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//����¼����±�־λ
	//ʹ��TIM2
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


