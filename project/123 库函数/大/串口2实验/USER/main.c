#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
int main(void)
{		
	u8 i=0x00;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart2_init(115200);	 				//���ڳ�ʼ��Ϊ115200
	LED_Init();			     			//LED�˿ڳ�ʼ��
	KEY_Init();          				//��ʼ���밴�����ӵ�Ӳ���ӿ�
	while(1)
	{
		USART_SendData(USART2,i++);
		delay_ms(1000);
	}
}

