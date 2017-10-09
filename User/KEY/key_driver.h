#ifndef _KEY_DRIVER_H_
#define _KEY_DRIVER_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

typedef enum
{
	MSG_KEY0 = 0,
	MSG_KEY1,
	MSG_KEY2,
	MSG_KEY3,
	
	NO_KEY = 0xff,
}__KEY_TYPE;

void Key_InitConfig(void);
void Key_delay(void);
__KEY_TYPE Get_KeyType(void);
void Key_Scan(__KEY_TYPE key_type);

#endif
