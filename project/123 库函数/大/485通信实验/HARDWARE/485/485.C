#include "485.h"
#include "delay.h"

#ifdef EN_USART2_RX

u8 RS485_RX_BUF[64];	/*接收缓存数组*/
u8 RS485_RX_CNT = 0;		/*接收数据字节多少计数*/

void USART2_IRQHandler(void)
{
	u8 res;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		res = USART_ReceiveData(USART2);
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT] = res;
			RS485_RX_CNT++;
		}
	}
}
#endif

void RS485_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART2_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOG,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位
	
#ifdef EN_USART2_RX
	
	USART2_InitStructure.USART_BaudRate = bound;
	USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART2_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART2_InitStructure.USART_Parity = USART_Parity_No;
	USART2_InitStructure.USART_StopBits = USART_StopBits_1;
	USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART2_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	
	USART_Cmd(USART2,ENABLE);
#endif

	RS485_TX_EN = 0;//初始化默认设置为接收模式
}
void RS485_Send_Data(u8 *buf,u8 length)
{
	u8 i;
	RS485_TX_EN = 1;
	for(i=0; i<length; i++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
		USART_SendData(USART2,buf[i]);
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
	RS485_RX_CNT = 0;
	RS485_TX_EN = 0;
}
void RS485_Receive_Data(u8 *buf,u8 *length)
{
	u8 rxlen = RS485_RX_CNT;
	u8 i = 0;
	*length = 0;
	delay_ms(10);
	if(rxlen == RS485_RX_CNT&&rxlen)
	{
		for(i=0; i<rxlen; i++)
		{
			buf[i] = RS485_RX_BUF[i];
		}
		*length = RS485_RX_CNT;
		RS485_RX_CNT = 0;
	}
}
