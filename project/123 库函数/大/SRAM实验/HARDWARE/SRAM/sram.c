#include "sram.h"

#define Bank1_Sram3_Address ((u32)(0x68000000))

void SRAM_FSMC_Init(void)
{
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef ReadWriteTiming;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = 0XFF33;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 0XFF83;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = 0XF03F;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = 0X163F;//将块1的四个区端口全部配置
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	ReadWriteTiming.FSMC_AddressSetupTime = 0X00;
	ReadWriteTiming.FSMC_AddressHoldTime = 0X00;
	ReadWriteTiming.FSMC_DataSetupTime = 0X03;
	ReadWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;
	ReadWriteTiming.FSMC_BusTurnAroundDuration = 0X00;
	ReadWriteTiming.FSMC_CLKDivision = 0X00;
	ReadWriteTiming.FSMC_DataLatency = 0X00;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;//修改此处 改变要使用的区
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &ReadWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &ReadWriteTiming;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3,ENABLE);//使能要使用的分区
}
void SRAM_FSMC_Write(u8 *pBuffer,u32 address,u32 n)
{
	for( ; n!=0; n--)
	{
		*(vu8 *)(Bank1_Sram3_Address+address) = *pBuffer;
		address++;
		pBuffer++;
	}
}
void SRAM_FSMC_Read(u8 *pBuffer,u32 address,u32 n)
{
	for( ; n!=0; n--)
	{
		*pBuffer++ = *(vu8 *)(Bank1_Sram3_Address+address);
		address++;
	}
}
void SRAM_FSMC_TEST_WRITE(u32 addr)
{
	u8 data;
	SRAM_FSMC_Write(&data,addr,1);
}
u8 SRAM_FSMC_TEST_READ(u32 addr)
{
	u8 data;
	SRAM_FSMC_Read(&data,addr,1);
	return data;
}
