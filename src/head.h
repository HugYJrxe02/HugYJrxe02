#ifndef HEAD_H
#define HEAD_H

#include <stm32f4xx.h>	
#include <stm32f4xx_can.h>

void delay_ms(uint32_t n);

void usart1_init(uint32_t baud);
void usart3_init(uint32_t baud);

void CAN_Config(void);
void Can_Send_Msg(CanTxMsg* TxMessage);

void init_PC_out(void);
void init_PE_out(void);
void init_PB_out(void);
void init_PD_out(void);

void TIM3_Init(void);
void TIM4_Init(void);

void SystemClock_Config(void);

int StrToHex(char *str, int bit_num);

#endif
