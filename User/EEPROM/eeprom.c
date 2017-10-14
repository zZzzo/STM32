#include "eeprom.h"
#include "i2c_driver.h"
#include "usart_driver.h"
#include "SysTick.h"


/*
*********************************************************************************************************
*	�� �� ��: ee_CheckOk
*	����˵��: �жϴ���EERPOM�Ƿ�����
*	��    �Σ���
*	�� �� ֵ: 1 ��ʾ������ 0 ��ʾ������
*********************************************************************************************************
*/
uint8_t eeprom_CheckOk(void)
{
	if (I2C_CheckDevice(EE_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		/* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
		I2C_Stop();		
		return 0;
	}
}

/******************************************************************
*	�� �� ��: eeprom_ReadBytes
*	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
********************************************************************/
uint8_t eeprom_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
	
	/* ��1��������I2C���������ź� */
	I2C_Start();
	
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	I2C_SendByte(EE_DEV_ADDR | I2C_WR);	/* �˴���дָ�� */
	I2C_SendByte((0XA0|((_usAddress/256)<<1))|I2C_WR);
//	I2C_SendByte((0XA0|(((_usAddress>>8)&0x01)<<1))|I2C_WR);
	
	/* ��3��������ACK */
	if (I2C_RecvACK() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
//	I2C_SendByte((uint8_t)_usAddress);
	I2C_SendByte((uint8_t)_usAddress%256);
//	I2C_SendByte((uint8_t)_usAddress&0xff);
	
	/* ��5��������ACK */
	if (I2C_RecvACK() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
	I2C_Start();
	
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	I2C_SendByte(EE_DEV_ADDR | I2C_RD);	/* �˴��Ƕ�ָ�� */
	I2C_SendByte((0XA0|((_usAddress/256)<<1))|I2C_RD);
	/* ��8��������ACK */
	if (I2C_RecvACK() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}	
	
	/* ��9����ѭ����ȡ���� */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = I2C_RecvByte();	/* ��1���ֽ� */
		
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != _usSize - 1)
		{
			I2C_SendACK(0);	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			I2C_SendACK(1);	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	}
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();
	return 1;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();
	printf("Read fild!\r\n");
	return 0;
}

/**********************************************************************************
*	�� �� ��: eeprom_WriteBytes
*	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
**********************************************************************************/
uint8_t eeprom_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;//�����һ��Ҫд�����ݵĵ�ַ
	
	/* 
		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
		����24xx04��page size = 16
		�򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ûд1���ֽڣ������͵�ַ
		Ϊ���������д��Ч��: ����������page wirte������
	*/
	

	usAddr = _usAddress;
	for (i = 0; i < _usSize; i++)
	{
		/* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
		if ((i == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
		{
			/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
			I2C_Stop();
			
			/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
				CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
			*/
			for (m = 0; m < 100; m++)
			{				
				/* ��1��������I2C���������ź� */
				I2C_Start();
				
				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//				I2C_SendByte(EE_DEV_ADDR | I2C_WR);	/* �˴���дָ�� */
				I2C_SendByte((0XA0|((usAddr/256)<<1))|I2C_WR);
				
				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if (I2C_RecvACK() == 0)
				{
					break;
				}
			}
//			if (m  == 100)
//			{
//				goto cmd_fail;	/* EEPROM����д��ʱ */
//			}
		
			/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ�1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
			
			I2C_SendByte((uint8_t)usAddr%256);
//			I2C_SendByte((uint8_t)usAddr);
			
			/* ��5��������ACK */
			if (I2C_RecvACK() != 0)
			{
				goto cmd_fail;	/* EEPROM������Ӧ�� */
			}
		}
	
		/* ��6������ʼд������ */
		I2C_SendByte(_pWriteBuf[i]);
	
		/* ��7��������ACK */
		if (I2C_RecvACK() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}

		usAddr++;	/* ��ַ��1 */		
	}
	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	I2C_Stop();
	return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	I2C_Stop();
	printf("Write fild!\r\n");
	return 0;
}



void eeprom_Erase(void)
{
	uint16_t i;
	uint8_t buf[EE_SIZE];
	
	/* ��仺���� */
	for (i = 0; i < EE_SIZE; i++)
	{
		buf[i] = 0xff;
	}
	
	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 512 */
	if (eeprom_WriteBytes(buf, 0, EE_SIZE) == 0)
	{
		printf("Erase eeprom error\r\n");
		return;
	}
	else
	{
		printf("Erase eeprom Success\r\n");
	}
}

uint8_t write_buf[EE_SIZE];
uint8_t read_buf[EE_SIZE];
/*
 * eeprom AT24C02 ��д����
 */
void eeprom_Test(void)
{
	uint16_t i;
//	uint8_t buff[20] = {0XAA,0X3B,0X2B,0X4C,0X45,0X18,0X97,0XF1,0XE5,0X69,
//						0X88,0X9D,0X3A,0X62,0XDE,0XCF,0X38,0X29,0X1A,0XBB};
	
	
	if(eeprom_CheckOk() == 0)
	{
		/* û�м�⵽EEPROM */
		printf("Not check EEPROM!\r\n");

		while (1);	/* ͣ�� */
	}
  
	/* �����Ի����� */
	for (i = 0; i < EE_SIZE/2; i++)
	{		
		write_buf[i] = i;
	}
	for (i = 0; i < EE_SIZE/2; i++)
	{		
		write_buf[256+i] = i;
	}
	
  
	if (eeprom_WriteBytes(write_buf, 0, EE_SIZE) == 0)
	{
		printf("Write eeprom error\r\n");
		return;
	}
	else
	{		
		printf("Write eeprom Success��\r\n");
	}
	
//	eeprom_WriteBytes(buff, 0X1AC, 20);

//	eeprom_Erase();

	/*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/
	delay_ms(20);
	/*-----------------------------------------------------------------------------------*/
	if (eeprom_ReadBytes(read_buf, 0, EE_SIZE) == 0)
	{
		printf("Read eeprom error\r\n");
		return;
	}
	else
	{
		printf("Read  eeprom Success\r\n");
	}
  
	for (i = 0; i < EE_SIZE; i++)
	{
		printf(" %02X", read_buf[i]);

		if ((i & 15) == 15)
		{
			printf("\r\n");	
		}
	}
}


