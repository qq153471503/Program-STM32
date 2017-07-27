/*串口1的简单使用*/
/*发送一个数据 返回接受的数据*/

#include"stm32f10x.h"
#include"usart.h"
#include "delay.h"


void Usart_Write(unsigned char dat)
{
	 USART1->SR;
//	USART_SendData(USART1,(unsigned char)dat);
//	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, (unsigned char) dat);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
}

int main(void)
{
	int i=0;
	delay_init();
	uart_init(9600);
	
	while(1){
		Usart_Write(i++);
		delay_ms(500);
	}
	
}

