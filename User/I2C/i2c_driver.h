#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_

#include "stm32f10x.h"
#include <inttypes.h>

#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

void I2C_GPIOInitConfig(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendACK(u8 ack);
u8 I2C_RecvACK(void);

void I2C_SendByte(u8 dat);
u8 I2C_RecvByte(void);

u8 I2C_CheckDevice(u8 _Address);

#endif
