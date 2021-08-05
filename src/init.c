#include <stm32f4xx.h>	// STM32F4系列芯片头文件
#include <stdio.h>
#include <string.h>
////毫秒级延时
void delay_ms(uint32_t n)
{
	//168MHz
	while(n--)
	{
		SysTick->CTRL = 0;			//	关闭系统定时器
		SysTick->LOAD = 168000-1;	//	计数值：16800 cycles
		SysTick->VAL = 0;			//	清空当前计数值
		SysTick->CTRL = 5;			//	选择168MHz 并打开定时器
		
		while((SysTick->CTRL & 0x00010000) == 0){}
			SysTick->CTRL = 0;			//	关闭系统定时器
	}
}



void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 使能定时器3时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* 配置定时器3 */
	TIM_TimeBaseStructure.TIM_Period = 10000-1;		//计数周期
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//预分频系数（10000Hz=10kHz）
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//时钟分割（F407无效）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式：向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	// 使能中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	/* 配置定时器中断优先级 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 打开定时器3 */
	TIM_Cmd(TIM3, ENABLE);
}

void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 使能定时器3时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* 配置定时器3 */
	TIM_TimeBaseStructure.TIM_Period = 10-1;		//计数周期
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;	//预分频系数（10000Hz=10kHz）
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//时钟分割（F407无效）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式：向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// 使能中断
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	/* 配置定时器中断优先级 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 打开定时器3 */
	TIM_Cmd(TIM4, ENABLE);
}

void init_PC_out()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* 1.使能对应GPIO的RCC时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);

}

void init_PB_out(void)
{
	//CAN_PW_EN
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* 1.使能对应GPIO的RCC时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);

}

void init_PD_out(void)
{
	//PD4 CAN_RS
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* 1.使能对应GPIO的RCC时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//输出速率（功耗）
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, GPIO_Pin_5);

}

void init_PE_out(void)
{
	//	定义一个结构体
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* GPIOG Peripheral clock enable  使能对应的GPIO的RCC时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* Configure PG6 and PG8 in output pushpull mode  配置GPIO并初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//	引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						//	输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//	推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//	输出速率
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;					//	无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//	将四个灯熄灭
	//GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	
}



void usart1_init(uint32_t baud)	
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//	1、使能端口、串口1的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//	2、配置GPIO为复用功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		//	引脚号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//	复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//	推挽模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//	输出速率
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//	无上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//	3、将引脚连接到对应复用功能引脚
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//	4、配置串口信息：波特率、数据为、校验位
	USART_InitStructure.USART_BaudRate = baud;							//	波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//	8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//	1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;					//	无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//	模式：收发
	USART_Init(USART1, &USART_InitStructure);

	//	使能串口1接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//	5、配置NVIC中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//	6、使能串口
	USART_Cmd(USART1, ENABLE);
}

void usart3_init(uint32_t baud)
{
	//PB10 TX PB11 RX
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	//	使能串口3接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART3, ENABLE);
	
}


void CAN_Config(void)
{
	//PD0 RX PD1 TX h
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	/* Configure CAN RX and TX pins */
	//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

//	/* CAN register init */
//	CAN_DeInit(CAN1);
  
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;//软件自动离线管理
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;


	/* CAN Baudrate = 500kbps (CAN clocked at 42 MHz) */
	/* 42000 / (6 + 7 + 1) / 6*/
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler = 6;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;  
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
}

void Can_Send_Msg(CanTxMsg* TxMessage)
{
	uint8_t mbox;

	mbox = CAN_Transmit(CAN1, TxMessage);
	uint32_t i = 0;
	while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i++ < 0xFFFF));
}

void SystemClock_Config(void)
{
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	while (RCC_WaitForHSEStartUp() != SUCCESS);

	uint32_t PLL_M = 8;
	uint32_t PLL_N = 336;
	uint32_t PLL_P = 2;
	uint32_t PLL_Q = 7;	
	
	RCC_HCLKConfig(RCC_HCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div4);
	RCC_PCLK2Config(RCC_HCLK_Div2);
	
	RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q);

	RCC_PLLCmd(ENABLE);

	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){} 
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}


int StrToHex(char *str, int bit_num)
{
	if (NULL == str)
	{
		return 0;
	}
	
	uint8_t strLen = bit_num - strlen (str);
	strLen = (strLen > 0) ? strLen : 0;
	uint8_t num = 0;
	
	for (int i = 0 ; i < bit_num && i < strlen (str); i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			num += ( str[i] - 0x30 ) << ( bit_num - i - 1 - strLen) * 4;
		}
		else if (str[i] >= 'A' && str[i] <= 'Z')
		{
			num += ( str[i] - 0x41 + 10) << ( bit_num - i - 1 - strLen) * 4;
		}
		else if (str[i] >= 'a' && str[i] <= 'z')
		{
			num += ( str[i] - 0x61 + 10) << ( bit_num - i - 1 - strLen) * 4;
		}
	}
	return num;
}



