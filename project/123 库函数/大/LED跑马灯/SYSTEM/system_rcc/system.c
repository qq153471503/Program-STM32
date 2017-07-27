#include "sys.h"
//版本：寄存器
//函数功能：初始化系统时钟
//函数简介：使用内部高速时钟HSI作为时钟源
//SYSTEM CLOCK : 64Mhz
//void RCC_Config(void)
//{
///*--------------------------------------# reset about rcc register #---------------------------------------------*/
//	/* Set HSION bit */
//	RCC->CR |= (uint32_t)0x00000001;
//	/* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
//	RCC->CFGR &= (uint32_t)0xF8FF0000;
//	/* Reset HSEON, CSSON and PLLON bits */
//	RCC->CR &= (uint32_t)0xFEF6FFFF;
//	/* Reset HSEBYP bit */
//	RCC->CR &= (uint32_t)0xFFFBFFFF;
//	/* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
//	RCC->CFGR &= (uint32_t)0xFF80FFFF;
//	/* Disable all interrupts and clear pending bits  */
//	RCC->CIR = 0x009F0000;
///*--------------------------------------------------------------------------------------------------------------*/
//	RCC->CR |= (uint32_t)0x00000001;								//使能HSI时钟
//	RCC->CFGR |= (uint32_t)0x00000000;     		 					//复位一下 无此句也可工作 带此句程序执行更严谨
//	RCC->CFGR |= (uint32_t)RCC_CFGR_PLLMULL16;						//16倍频 8/2*16=64Mhz
//	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_2; 						//AHB=SYSTEM APB2=SYSTEM APB1=SYSTEM/2
//	FLASH->ACR|= (uint32_t)0X000000032;	  							//切换时钟源 需要FLASH 1或2个延时周期 此处选两个
//	RCC->CR |= (uint32_t)RCC_CR_PLLON;								//使能PLL
//	while((RCC->CR&(uint32_t)RCC_CR_PLLRDY) == 0);					//等待使能完成
//	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));				//复位一下最低两位(最低两位是系统时钟切换) 无此句也可工作 带此句程序执行更严谨
//	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;							//设置PLL输出作为系统时钟
//	while((RCC->CFGR&(uint32_t)RCC_CFGR_SWS)!= (uint32_t)0x08);		//等待系统时钟切换完成
///*****************************************************************************************************************/
//}

//版本：库函数
//函数功能：初始化系统时钟
//函数简介：使用内部高速时钟HSI作为时钟源
//SYSTEM CLOCK : 64Mhz
void RCC_Config(void)
{
	RCC_DeInit();													//将RCC相关寄存器重设为缺省值
	RCC_HSICmd(ENABLE);												//使能HSI时钟
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16);			//设置PLL时钟源为HSI 16倍频
	FLASH_SetLatency(FLASH_Latency_2);								//切换时钟源 FLASH需要1或2个延时周期 此处选两个
	RCC_PLLCmd(ENABLE);												//使能PLL时钟源
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);				//等待使能完成
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);						//设置系统时钟源选择PLL
	while(RCC_GetSYSCLKSource() != 0x08);							//等待PLL锁定
}
/*#######################################################################################################################*/
