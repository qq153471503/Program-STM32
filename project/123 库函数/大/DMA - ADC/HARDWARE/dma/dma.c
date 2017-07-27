#include "dma.h"

u16 ADC_Data;/*用于存放ADC检测后的值*/

extern u8 SendBuff[];

void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

/*****************************************************************/
//此函数块是设置ADC检测后的值存入内存	
//一次发送的数据大小为半个字。
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Data;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
/******************************************************************************/
//此函数块是ADC检测后存入内存的值 送向 串口1
//一次发送的数据大小为字节。
	DMA_DeInit(DMA1_Channel4);	
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&SendBuff;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_BufferSize = 4;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel4,&DMA_InitStructure);
/*******************************************************************************/
}
/*========================================================================*/
//参数1：DMA通道选择
//x ：一次传送的数据长度
void DMA_Enable(DMA_Channel_TypeDef * DMA_CHx,u8 x)
{
	DMA_Cmd(DMA_CHx,DISABLE);
	DMA_SetCurrDataCounter(DMA_CHx,x);
	DMA_Cmd(DMA_CHx,ENABLE);
}
