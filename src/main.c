
#include "stm32f4xx.h"
#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma import(__use_no_semihosting_swi)
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) 
{
	//重定向输出到串口1
	USART_SendData(USART3, ch);
	
	// 等待串口发送成功
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	
	return ch;
}
void _sys_exit(int return_code) {
label: goto label; /* endless loop */
}


#define PCout(x) *(uint32_t *)(0x42000000+(GPIOC_BASE+0x14-0x40000000)*32+x*4)
#define PEout(x) *(uint32_t *)(0x42000000+(GPIOE_BASE+0x14-0x40000000)*32+x*4)

typedef enum {False = 0, True = 1}bool;


volatile bool Can_Rx_Flag = False;
volatile bool Can_Tx_Flag = False;

volatile uint32_t timeInterval = 0;//帧间隔 时间

static CanRxMsg RxMessage;
static CanTxMsg TxMessage;

/* 
 * 0x00 空闲状态
 * 0x01 正在接收can数据
 * 0x02 正在接收usart3数据
*/
static volatile uint8_t RcvType = 0x00;

static uint8_t usart_rcv_index = 0;//控制usart3接收到数据存放到rcv_msg_buf位置
static char rcv_msg_buf[9][4] = {0};//缓存 usart3接收到的can数据 
static bool space_rcv_flag = False; // 收到空格标志


int main(void)
{
	SystemClock_Config();//系统时钟配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 选择中断优先级4组
	
	usart3_init(115200);////PB10 TX , PB11 RX
	CAN_Config();//PD0 RX CANL , PD1 TX CANH
	
	init_PB_out();//CAN_PW_EN
	init_PC_out();//WDG  PC1
	
	TIM3_Init();//外设定时器1秒钟喂狗
	TIM4_Init();//外设定时器4毫秒计数
	
	while (1)
	{
		if (Can_Rx_Flag)//单片机收到can数据
		{
			printf ("timeInterval: %dms\n", timeInterval);
			printf ("ID: %04x, msg:%04x, %04x, %04x, %04x, %04x, %04x, %04x, %04x\n",  RxMessage.StdId, RxMessage.Data[0], RxMessage.Data[1], RxMessage.Data[2], RxMessage.Data[3], RxMessage.Data[4], RxMessage.Data[5], RxMessage.Data[6], RxMessage.Data[7]);
			Can_Rx_Flag = False;
			timeInterval = 0;	
		}
		if (Can_Tx_Flag)//将usart接收的can数据转发到can1
		{
			/*! Specifies the standard identifier.This parameter can be a value between 0 to 0x7FF. */
			TxMessage.StdId = StrToHex(rcv_msg_buf[0], 3);
			TxMessage.ExtId = 0x02;
			TxMessage.RTR = CAN_RTR_DATA;
			TxMessage.IDE = CAN_ID_STD;
			TxMessage.DLC = 8;
			for (uint8_t i = 0; i < 8; i++)
			{
				TxMessage.Data[i] = StrToHex(rcv_msg_buf[i+1], 2);
			}
			Can_Send_Msg(&TxMessage);
			Can_Tx_Flag = False;
		}			
	}
}

//CAN1 接收中断
void CAN1_RX0_IRQHandler(void)
{
	memset(&RxMessage, 0, sizeof(RxMessage));
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	Can_Rx_Flag = True;
}


// TIM3中断服务函数 硬件喂狗
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		PCout(1) = ~PCout(1);//硬件喂狗
	}
}



// TIM3中断服务函数 毫秒计数->计算接收的can帧间隔
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		timeInterval++;
	}
}

/* USART3中断 */
void USART3_IRQHandler(void)
{
	uint8_t rcv = 0;
	uint8_t i = 0;
	//	检测是否接收到数据
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		//	接收读取到的数据	
		rcv = USART_ReceiveData(USART3);
	 
		if (0x00 == RcvType)
		{
			/* S 7E8 00 00 00 00 00 00 00 00 #*/
			if ('S' == rcv)
			{
				space_rcv_flag = True;
				usart_rcv_index = 1;
				memset(rcv_msg_buf, 0, sizeof(rcv_msg_buf));
				memset(&TxMessage, 0, sizeof(TxMessage));
				RcvType = 0x01;
			}	
			else if ('U' == rcv)
			{
				RcvType = 0x02;
			}
		}
		/* 正在接收can数据 */
		else if (0x01 == RcvType)
		{
			if ('#' == rcv)
			{
				space_rcv_flag = False;
				Can_Tx_Flag = True;
				RcvType = 0x00;
			}
			else if (' ' == rcv)
			{
				if (!space_rcv_flag)
				{
					usart_rcv_index += 1;
				}
				space_rcv_flag = True;
			}
			else if (usart_rcv_index < 10)
			{
				space_rcv_flag = False;
				if (1 == usart_rcv_index)
				{
					i = 0;
					while (rcv_msg_buf[usart_rcv_index-1][i] && i < 2)
					{ i++; }
					rcv_msg_buf[usart_rcv_index-1][i] = rcv;	
				}
				else
				{
					i = 0;
					while (rcv_msg_buf[usart_rcv_index-1][i] && i < 1)
					{ i++; }
					rcv_msg_buf[usart_rcv_index-1][i] = rcv;				
				}	
			}		
		}
		

		
		/* 正在接收usart3数据 */
		else if (0x02 == RcvType)
		{
			if ('#' == rcv)
			{
				RcvType = 0x00;
			}
			else 
			{
				USART_SendData(USART3, rcv);
				while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
			}
		}
		
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
		//	清空标志位
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
