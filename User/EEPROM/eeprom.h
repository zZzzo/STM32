#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "stm32f10x.h"

#define EE_DEV_ADDR			0xA0		/* 24xx04的设备地址 */
#define EE_PAGE_SIZE		16			  /* 24xx04的页面大小 */
#define EE_SIZE				512			  /* 24xx04总容量 */


uint8_t eeprom_CheckOk(void);
uint8_t eeprom_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t eeprom_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void eeprom_Erase(void);
void eeprom_Test(void);


#endif
