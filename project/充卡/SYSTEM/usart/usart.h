#ifndef __USART_H
#define __USART_H
#include <stm32f10x_lib.h>
#include "stdio.h"	 

//Mini STM32开发板
//串口1初始化	

#define  MAX_NUM  40		   
 
extern u8 USART1_RX_BUF[];     //接收缓冲  	
extern u8 USART2_RX_BUF[];     //接收缓冲
extern u8 USART_RX_STA;         //接收状态标记	
extern u8 flag1,flag2,rx2zt_flag;
extern u8 flag3;
extern u8 rxd1_over_flag;
extern u8 USART1_RXNUM;
extern void UART_Send_Str(char *s);
extern int U1SendChar(int ch);
extern int U2SendChar(int ch);
extern int GetKey(void);

void U1Send_String( u8 *s);

#define EN_USART1_RX 			0		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口2接收	
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口3接收

void uart1_init(u32 bound);
void uart2_init(u32 bound ,u8 mode);

#endif	   
















