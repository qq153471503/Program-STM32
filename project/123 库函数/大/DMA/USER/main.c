#include "lcd.h"
#include "dma.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"

#define Send_Buff_Size 9600  /*������ַ����鳤�ȵ�������*/
u8 SendBuff[Send_Buff_Size];
const u8 Send_Data[] = {"ս�������� DMA����ʵ��"};/*22*/  /*��ӻس� ���к� *  ��25��  4800/25=192*/
int main(void)
{
	u8 changdu;
	u16 i;
	u8 t = 0,mask = 0;
	float temp;
	
	SystemInit();
	delay_init();
	uart_init(115200);
	KEY_Init();
	LED_Init();
	LCD_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,Send_Buff_Size);
	
	POINT_COLOR = RED;
	LCD_ShowString(80,20,72,16,16,"DMA TEST!");
	LCD_ShowString(80,40,144,16,16,"Please enter KEY0:");
	
	changdu = sizeof(Send_Data);  //���������С
	
	for(i=0; i<Send_Buff_Size; i++)
	{
		if(t>=changdu)
		{
			if(mask)
			{
				SendBuff[i] = 0x0a;		//����		���ߴ�ӡ�������ƶ�һ��
				t = 0;
			}else						//������ο�http://www.ruanyifeng.com/blog/2006/04/post_213.html
			{
				SendBuff[i] = 0x0d;		//�س�  ���ߴ�ӡ������궨λ����߽�
				mask++;
			}
		}
		else
		{
			mask = 0;
			SendBuff[i] = Send_Data[t];
			t++;
		}
	}
		
	while(1)
	{
		POINT_COLOR = BLACK;
		
		if(KEY0_PRES == (KEY_Scan(0)))
		{
			LCD_ShowString(80,60,152,16,16,"Start Transimit....");
			LCD_ShowString(80,80,32,16,16,"   %");
			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
			DMA_Enable(DMA1_Channel4);
			
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
				{
					DMA_ClearFlag(DMA1_FLAG_TC4);
					break;
				}
				
				temp = DMA_GetCurrDataCounter(DMA1_Channel4);
				temp = 1-temp/Send_Buff_Size;
				temp*=100;
				LCD_ShowNum(80,80,temp,3,16);
			}
			LCD_ShowString(80,60,152,16,16,"Transimit Finishde!");
			LCD_ShowNum(80,80,100,3,16);
		}
			
	}
	
}
