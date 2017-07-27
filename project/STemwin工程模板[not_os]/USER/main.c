#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "ILI93XX.h"
#include "usart.h"	  
#include "sram.h"
#include "malloc.h"
#include "string.h"
#include "usmart.h"	
#include "GUI.H"

 int main(void)
 {	 
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	TFTLCD_Init();			   		//��ʼ��LCD   
 	FSMC_SRAM_Init();			//��ʼ���ⲿSRAM  
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��

	RCC_AHBPeriphClockCmd(RCC_AHBENR_CRCEN, ENABLE);	//��CRCʱ��
	GUI_Init();
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_Font32_ASCII);
	GUI_Clear();
	GUI_DispStringAt("Hello STemwin!",10, 10);
  	while(1)
	{	
		
	}
}
 



