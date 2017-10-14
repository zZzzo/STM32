
No1：
	GPIO口的使用-->寄存器/库函数
	GPIO口的8种模式，使用场合...
	寄存器其实就是MCU/CPU的某个物理空间地址
	例如	
	GPIOC->CRL &= ~(0xFF<<24);
	GPIOC->CRL |= 0x33<<24;
	相当于：
	*((unsigned long *)0x40011000) &= ~(0xFF<<24);
	*((unsigned long *)0x40011000) |= 0x33<<24;
No2:
	SysTick系统滴答定时器(24位向下计数器)
	启动过程
	1选择时钟源
	2根据选择的时钟源设定load的倍数
	3装载计数次数
	4计数器清零(VAL清零)，因为current字段被手动清零时，LOAD将自动装载到VAL中。
	5使能
	注意的点：ms的最大延时时间：nms<=1864
	
No2.1 STM32定时器：
	STM32定时器(11个)：TIM1~TIM8,IWDG,WWDG,SYSTICK.
	TIM1和TIM8高级控制定时器：伺服电机，步进电机...
	TIM6和TIM7基本定时器：也可以产生PWM,还支持DAC
	TIMx通用定时器：用于产生PWM...
No2.2 TIMx通用定时器：
	(通过可编程预分频驱动的16(0~65535)位自动装载计数器构成
	，1.说明可以自定义计数频率，而SYSTICK定时器的工作频率是固定的HCLK/8
	，2.自动装载:不必在软件中装载计数值，SYSTICK每次计数都要手动去装载)
	预分频系数 = (通用定时器工作频率/计数频率 )-1
	72MHz/x -1 ,假设计数频率要设为10KHz,那么预分频系数就是7199
	计数完成-->(溢出)更新事件-->产生中断  TIM_IT_Update
	主要设置 预分频系数，要定时的时间，计数模式(向上还是向下还是中间)
	
No3:
	RCC(复位和时钟控制)
	函数 void RCC_Configuration(void)
	在3.0版本前的库中，是必须配置的，在3.0后出现了stm32f10x.h，将时钟配置定为默认的。
	在main函数中不直接使用这个函数了。
	配置时钟流程：
	1.将RCC寄存器重新设置为默认值	RCC_DeInit();
	2.打开外部高速时钟晶振	HSE
	3.等待HSE工作
	4.设置AHB时钟，设置APB2时钟，设置APB1时钟
	5.设置PLL(设为HSE的9倍频)
	6.使能PLL
	7.等待PLL使能完毕
	8.设置系统时钟等于PLL时钟	SYSCLK = PLL
	9.判断是否PLL是系统时钟

No4：
	外部中断EXTI
	由19个产生事件/中断的边沿检测器组成EXTI0....EXTI19
	EXTI0~EXTI15 16组分别对应{PA0 PB0...}{PA1 PB1...}...{PA15 PB15...}
	还有3组：
		EXTI16连接PVD输出
		EXTI17连接到RTC闹钟事件
		EXTI18连接到USB唤醒事件
	寄存器：
	中断/事件 0屏蔽1开放
	EXTI_IMR	中断屏蔽寄存器
	EXTI_EMR	事件屏蔽寄存器
	边沿选择
	EXTI_RTSR	上升
	EXTI_FTSR	下降
	
No5:
	并行通信接口和串行通信接口
	接线模型-->数据线就一根，一根发一根收
	1、指定起始/终止信号 起始下降沿
	2、之间发数据就要使用通信协议，指定一次发多少位，每位的含义是什么
	3、指定相同的波特率，说明每位信号的时间。
	
	MCU上两个口的模式设置：
	USART常用的库函数：
	void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
	void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
	void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);
	void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
	uint16_t USART_ReceiveData(USART_TypeDef* USARTx);
	void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG);

No5:
	NVIC嵌套向量中断控制器
	static __INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
	static __INLINE void NVIC_EnableIRQ(IRQn_Type IRQn)
	static __INLINE void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
	
	NIVC默认是组4，即4位抢占优先级，0位响应优先级
	实际使用默认的组4就够了，例子：
		NVIC_SetPriority(UART1_IRQn,2);
		NVIC_EnableIRQ(UART1_IRQn);
	数字越低越优先。
	
No6:
	I2C半双工串行同步总线
	I2C协议不是指的软件定义的协议，而是硬件规范。一般都有个I2C控制器控制。
	只需要读懂它的时序。
	本工程里程序使用IO口模拟I2C协议。
	I2C协议的时序，
	1、起始信号	SCL拉高，SDA下降沿
	2、终止信号	SCL拉高，SDA上升沿
		起始终止信号占用两种情况，所以数据传输时，SCL为低电平时，SDA才能跳变。SCL为高电平时，SDA要保持
	3、数据传输：(读/写)
		读：SCL拉高，读取SDA状态。
		写：SCL先拉低，SDA跳变(写)。
	4、应答信号(肯定应答/否定应答)
		ACK---- 0
		NACK--- 1
	协议时序的软件实现。
	
	
	
	