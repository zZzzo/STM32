#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define ON		0
#define OFF		1

typedef enum{
	LED_RED = 0,
	LED_GREEN,
	LED_BLUE,
}__LED_TYPE;



void Led_InitConfig(void);
void Led_Ctl(__LED_TYPE type,uint8_t statue);



#endif

