#include "dma.h"

u16 ADC_Data;/*���ڴ��ADC�����ֵ*/

extern u8 SendBuff[];

void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

/*****************************************************************/
//�˺�����������ADC�����ֵ�����ڴ�	
//һ�η��͵����ݴ�СΪ����֡�
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
//�˺�������ADC��������ڴ��ֵ ���� ����1
//һ�η��͵����ݴ�СΪ�ֽڡ�
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
//����1��DMAͨ��ѡ��
//x ��һ�δ��͵����ݳ���
void DMA_Enable(DMA_Channel_TypeDef * DMA_CHx,u8 x)
{
	DMA_Cmd(DMA_CHx,DISABLE);
	DMA_SetCurrDataCounter(DMA_CHx,x);
	DMA_Cmd(DMA_CHx,ENABLE);
}
