#include "stm32f10x.h"
#include "led_driver.h"
#include "SysTick.h"
#include "key_driver.h"
#include "sys_rcc.h"
#include "usart_driver.h"
#include "ST_String.h"
#include "tim_driver.h"
#include "exti_driver.h"
#include "i2c_driver.h"
#include "eeprom.h"
#include "spi_driver.h"
#include "oled_driver.h"
#include "adc_driver.h"
#include "dht11_driver.h"

int main(void)
{
//	char *buff = "usart1 test!  usart1_putstr\r\n";
//	uint16_t num = 5566;
	u8 temp = 0;
	u8 humidity = 0;
	
	
	RCC_DeInit();
	RCC_Configuration();
//	Led_InitConfig();		/*简单的三色LED*/
	LED_GPIO_InitConfig();	/*PWM控制的LED*/
	Key_InitConfig();
	SysTick_init(72);
	USART_InitConfig();
	TIM_InitConfig();
	EXTI_InitConfig();
	I2C_GPIOInitConfig();
	
	SPI_InitConfig();
	LCD_Init();
	
	ADC1_Init();
	DHT11_InitConfig();
	
#if 0
	/*****测试*****/
	//1、写字符
	usart1_putc('H');
	usart1_putc('e');
	usart1_putc('l');
	usart1_putc('l');
	usart1_putc('o');
	usart1_putc(' ');
	usart1_putc('W');
	usart1_putc('o');
	usart1_putc('r');
	usart1_putc('l');
	usart1_putc('d');
	usart1_putc('\r');
	usart1_putc('\n');
	
	//2、写字符串 (2种占用内存空间的区别)
	usart1_putstr(buff,st_strlen(buff));
	uasrt1_print("usart1 test!  usart1_print\r\n");
	printf("usart1 test!  usart1_printf\r\n");
	//5、读数据 阻塞--->读到A才继续
//	while(usart1_getc() != 'A');
	//3、写数字
	usart1_putc(num/1000 + '0');
	usart1_putc(num%1000/100 + '0');
	usart1_putc(num%100/10 + '0');
	usart1_putc(num%10 + '0');
	usart1_putc('\r');
	usart1_putc('\n');
	
	eeprom_Test();
	
#endif

	ReadDat(&temp,&humidity);
	printf("temp = %d\r\n",temp);
	printf("humidity = %d\r\n",humidity);

	OLED_ShowNum(20,2,temp,2,16);
	while(1)
	{
		if(ReadDat(&temp,&humidity) == 0)
			printf("readdat failure\r\n");
		else
		{
		LCD_CLS();
		OLED_ShowCHinese(16,0,0);
		OLED_ShowCHinese(32,0,1);
		OLED_ShowNum(50,0,temp,2,16);
		OLED_ShowCHinese(16,4,2);
		OLED_ShowCHinese(32,4,3);
		OLED_ShowNum(50,4,humidity,2,16);
		delay_ms(1000);
		delay_ms(1000);
		}
	}
	
	//4、重定向printf(2步:1.重定义fputc()函数；2.在配置中勾选Use MiroLIB)
//	#ifdef USART1_DEBUG
//	printf("num = %d\r\n",num);
//	#endif
#if 0
	while(1)
	{
		#if 0
		printf("CH10 ADC_VAL = %d   V = %f\r\n",Get_Adc(10),Get_AdcMath(Get_Adc(10)));
		printf("CH11 ADC_VAL = %d   V = %f\r\n",Get_Adc(11),Get_AdcMath(Get_Adc(11)));
		delay_ms(1000);
		BreadtheLight();
		
		#else
		//Key_Scan(Get_KeyType());
		#endif
		
		
	}
#endif
}


