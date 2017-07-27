#include "spi.h"
/*=================================================================================================*/
void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI2_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI2_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI2_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI2_InitStructure.SPI_CRCPolynomial = 7;
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2,&SPI2_InitStructure);
	
	SPI_Cmd(SPI2,ENABLE);
	SPI2_ReadWriteByte(0XFF);	/*启动传输*/
}
/*=================================================================================================*/
//读写一个字节的数据 
//txd：要发送的数据
u8 SPI2_ReadWriteByte(u8 txd)
{
	u8 count = 0;
	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET)
	{
		count++;
		if(count >= 200)
			return 0;
	}
	
	SPI_I2S_SendData(SPI2,txd);
	count = 0;
	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET)
	{
		count++;
		if(count >= 200)
			return 0;
	}
	
	return SPI_I2S_ReceiveData(SPI2);
}
/*=================================================================================================*/
//SPI2设置传输速度函数
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
/*CR1寄存器介绍：
位数：16
3 4 5 这三位是设置波特率使用
0XFFCF   高八位和第三位取余是用来保持原来的状态  3 4 5 位取余给0复位 然后 | 关系 只变更3 4 5 位*/
void SPI2_Set_Speed(u8 SPI_BaudRatePrescaler)
{
	SPI2 -> CR1 &= 0XFFC7;		
	SPI2 -> CR1 |= SPI_BaudRatePrescaler;
	SPI_Cmd(SPI2,ENABLE);
}
/*=================================================================================================*/


