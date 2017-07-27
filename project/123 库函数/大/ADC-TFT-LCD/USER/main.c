#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "adc.h"
//#include "pic.h" /*ͼƬ���� ����ʾͼƬ���ô�ͷ�ļ�*/
 
int main(void)
{	 
 	u16 temp;
	float zhuanhuan;
	
	u8 lcd_id[12];			//���LCD ID�ַ���
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	AdcInit();
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣				 	

	LCD_ShowString(50,50,64,16,16,"ADC-TEST");
	LCD_ShowString(50,70,80,16,16,"ADC Value:");
	LCD_ShowString(50,90,200,16,16,"Valtage:       V");
	
	while(1)
	{
		temp = GetAdcAverage(0,10);
	
		LCD_ShowNum(130,70,temp,4,16);
		
		zhuanhuan = (float)temp*(3.3/4096);
		temp = zhuanhuan;						//�Զ����˵�С��λ
		LCD_ShowxNum(130,90,temp,1,16,0);
		LCD_ShowChar(139,90,'.',16,0);
		
		zhuanhuan = (zhuanhuan-temp)*1000;
		
		LCD_ShowxNum(143,90,zhuanhuan,3,16,0x80);
		delay_ms(200);
	}
}
