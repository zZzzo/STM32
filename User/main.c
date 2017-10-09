#include "stm32f10x.h"
#include "led_driver.h"
#include "SysTick.h"
#include "key_driver.h"
#include "sys_rcc.h"


int main(void)
{
	RCC_DeInit();
	RCC_Configuration();
	
	Led_InitConfig();
	Key_InitConfig();
	SysTick_init(72);
	
	while(1)
	{
		Key_Scan(Get_KeyType());
	}	
}

