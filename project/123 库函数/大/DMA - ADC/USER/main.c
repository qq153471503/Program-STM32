#include "lcd.h"
#include "dma.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "adc.h"

u8 SendBuff[16];/*用于存放adc检测的值 DMA给串口的基地址*/

int main(void)
{	
	float voltage; /*存放单精度的电压值*/
	SystemInit();
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	KEY_Init();
	LED_Init();
	AdcInit();
	DMA_Config();

	while(1)
	{
		if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == SET)
		{
			ADC_ClearITPendingBit(ADC1,ADC_Channel_0);
			DMA_Enable(DMA1_Channel1,1);
			ADC_DMACmd(ADC1,ENABLE);
		}
		
		voltage = (float)ADC_Data*(3.3/4096);
		
			sprintf((char*)SendBuff,"\n\nADC Value:  %d\r\n",ADC_Data);/*将adc数据值打印机数组*/
			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
			DMA_Enable(DMA1_Channel4,20);
			
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
				{
					DMA_ClearFlag(DMA1_FLAG_TC4);
					break;
				}
			}			
		printf("Voltage:  %0.3fV\r\n",voltage);/*向串口打印电压值*/
		delay_ms(1000);
		delay_ms(1000);
	}
}
