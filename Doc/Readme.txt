
No1��
	GPIO�ڵ�ʹ��-->�Ĵ���/�⺯��
	GPIO�ڵ�8��ģʽ��ʹ�ó���...
	�Ĵ�����ʵ����MCU/CPU��ĳ������ռ��ַ
	����	
	GPIOC->CRL &= ~(0xFF<<24);
	GPIOC->CRL |= 0x33<<24;
	�൱�ڣ�
	*((unsigned long *)0x40011000) &= ~(0xFF<<24);
	*((unsigned long *)0x40011000) |= 0x33<<24;
No2:
	SysTickϵͳ�δ�ʱ��(24λ���¼�����)
	��������
	1ѡ��ʱ��Դ
	2����ѡ���ʱ��Դ�趨load�ı���
	3װ�ؼ�������
	4����������(VAL����)����Ϊcurrent�ֶα��ֶ�����ʱ��LOAD���Զ�װ�ص�VAL�С�
	5ʹ��
	ע��ĵ㣺ms�������ʱʱ�䣺nms<=1864
	
No2.1 STM32��ʱ����
	STM32��ʱ��(11��)��TIM1~TIM8,IWDG,WWDG,SYSTICK.
	TIM1��TIM8�߼����ƶ�ʱ�����ŷ�������������...
	TIM6��TIM7������ʱ����Ҳ���Բ���PWM,��֧��DAC
	TIMxͨ�ö�ʱ�������ڲ���PWM...
No2.2 TIMxͨ�ö�ʱ����
	(ͨ���ɱ��Ԥ��Ƶ������16(0~65535)λ�Զ�װ�ؼ���������
	��1.˵�������Զ������Ƶ�ʣ���SYSTICK��ʱ���Ĺ���Ƶ���ǹ̶���HCLK/8
	��2.�Զ�װ��:�����������װ�ؼ���ֵ��SYSTICKÿ�μ�����Ҫ�ֶ�ȥװ��)
	Ԥ��Ƶϵ�� = (ͨ�ö�ʱ������Ƶ��/����Ƶ�� )-1
	72MHz/x -1 ,�������Ƶ��Ҫ��Ϊ10KHz,��ôԤ��Ƶϵ������7199
	�������-->(���)�����¼�-->�����ж�  TIM_IT_Update
	��Ҫ���� Ԥ��Ƶϵ����Ҫ��ʱ��ʱ�䣬����ģʽ(���ϻ������»����м�)
	
No3:
	RCC(��λ��ʱ�ӿ���)
	���� void RCC_Configuration(void)
	��3.0�汾ǰ�Ŀ��У��Ǳ������õģ���3.0�������stm32f10x.h����ʱ�����ö�ΪĬ�ϵġ�
	��main�����в�ֱ��ʹ����������ˡ�
	����ʱ�����̣�
	1.��RCC�Ĵ�����������ΪĬ��ֵ	RCC_DeInit();
	2.���ⲿ����ʱ�Ӿ���	HSE
	3.�ȴ�HSE����
	4.����AHBʱ�ӣ�����APB2ʱ�ӣ�����APB1ʱ��
	5.����PLL(��ΪHSE��9��Ƶ)
	6.ʹ��PLL
	7.�ȴ�PLLʹ�����
	8.����ϵͳʱ�ӵ���PLLʱ��	SYSCLK = PLL
	9.�ж��Ƿ�PLL��ϵͳʱ��

No4��
	�ⲿ�ж�EXTI
	��19�������¼�/�жϵı��ؼ�������EXTI0....EXTI19
	EXTI0~EXTI15 16��ֱ��Ӧ{PA0 PB0...}{PA1 PB1...}...{PA15 PB15...}
	����3�飺
		EXTI16����PVD���
		EXTI17���ӵ�RTC�����¼�
		EXTI18���ӵ�USB�����¼�
	�Ĵ�����
	�ж�/�¼� 0����1����
	EXTI_IMR	�ж����μĴ���
	EXTI_EMR	�¼����μĴ���
	����ѡ��
	EXTI_RTSR	����
	EXTI_FTSR	�½�
	
No5:
	����ͨ�Žӿںʹ���ͨ�Žӿ�
	����ģ��-->�����߾�һ����һ����һ����
	1��ָ����ʼ/��ֹ�ź� ��ʼ�½���
	2��֮�䷢���ݾ�Ҫʹ��ͨ��Э�飬ָ��һ�η�����λ��ÿλ�ĺ�����ʲô
	3��ָ����ͬ�Ĳ����ʣ�˵��ÿλ�źŵ�ʱ�䡣
	
	MCU�������ڵ�ģʽ���ã�
	USART���õĿ⺯����
	void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
	void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
	void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);
	void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
	uint16_t USART_ReceiveData(USART_TypeDef* USARTx);
	void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG);

No5:
	NVICǶ�������жϿ�����
	static __INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
	static __INLINE void NVIC_EnableIRQ(IRQn_Type IRQn)
	static __INLINE void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
	
	NIVCĬ������4����4λ��ռ���ȼ���0λ��Ӧ���ȼ�
	ʵ��ʹ��Ĭ�ϵ���4�͹��ˣ����ӣ�
		NVIC_SetPriority(UART1_IRQn,2);
		NVIC_EnableIRQ(UART1_IRQn);
	����Խ��Խ���ȡ�
	
No6:
	I2C��˫������ͬ������
	I2CЭ�鲻��ָ����������Э�飬����Ӳ���淶��һ�㶼�и�I2C���������ơ�
	ֻ��Ҫ��������ʱ��
	�����������ʹ��IO��ģ��I2CЭ�顣
	I2CЭ���ʱ��
	1����ʼ�ź�	SCL���ߣ�SDA�½���
	2����ֹ�ź�	SCL���ߣ�SDA������
		��ʼ��ֹ�ź�ռ������������������ݴ���ʱ��SCLΪ�͵�ƽʱ��SDA�������䡣SCLΪ�ߵ�ƽʱ��SDAҪ����
	3�����ݴ��䣺(��/д)
		����SCL���ߣ���ȡSDA״̬��
		д��SCL�����ͣ�SDA����(д)��
	4��Ӧ���ź�(�϶�Ӧ��/��Ӧ��)
		ACK---- 0
		NACK--- 1
	Э��ʱ������ʵ�֡�
	
	
	
	