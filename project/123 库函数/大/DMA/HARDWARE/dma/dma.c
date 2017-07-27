#include "dma.h"

u16 DMA_Length;

void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 p,u32 m,u16 length)
{
	DMA_InitTypeDef DMA_InitStructure;	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_DeInit(DMA_CHx);

	DMA_Length = length;
	
	DMA_InitStructure.DMA_MemoryBaseAddr = m;
	DMA_InitStructure.DMA_PeripheralBaseAddr = p;
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	DMA_InitStructure.DMA_BufferSize = length;
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(DMA_CHx,&DMA_InitStructure);
}
void DMA_Enable(DMA_Channel_TypeDef * DMA_CHx)
{
	DMA_Cmd(DMA_CHx,DISABLE);
	DMA_SetCurrDataCounter(DMA_CHx,DMA_Length);
	DMA_Cmd(DMA_CHx,ENABLE);
}
