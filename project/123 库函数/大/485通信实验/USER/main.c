#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "rs485.h"
//ALIENTEKս��STM32������ʵ��24
//485 ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  
 				 	
 int main(void)
 {	 
	u8 key;
	u8 i=0,t=0;
	u8 cnt=0;
	u8 rs485buf[5];
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
 	LedInit();			     //LED�˿ڳ�ʼ��
	LCD_Init();			     //LCD��ʼ��
	KeyInit();	 			 //KEY��ʼ��
	RS485_Init(9600);	//��ʼ��RS485

 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"RS485 TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2012/9/9");	
	LCD_ShowString(60,130,200,16,16,"KEY0:Send");	//��ʾ��ʾ��Ϣ	
 
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
	LCD_ShowString(60,150,200,16,16,"Count:");			//��ʾ��ǰ����ֵ	
	LCD_ShowString(60,170,200,16,16,"Send Data:");		//��ʾ���͵�����	
	LCD_ShowString(60,210,200,16,16,"Receive Data:");	//��ʾ���յ�������		
 									  
	while(1)
	{
		key=KeyScan(0);
		if(key==KEY_RIGHT)//KEY0����,����һ������
		{
			for(i=0;i<5;i++)
			{
				rs485buf[i]=cnt+i;//��䷢�ͻ�����
				LCD_ShowxNum(60+i*32,190,rs485buf[i],3,16,0X80);	//��ʾ����
 			}
			RS485_Send_Data(rs485buf,5);//����5���ֽ� 									   
		}		 
		RS485_Receive_Data(rs485buf,&key);
		if(key)//���յ�������
		{
			if(key>5)key=5;//�����5������.
 			for(i=0;i<key;i++)LCD_ShowxNum(60+i*32,230,rs485buf[i],3,16,0X80);	//��ʾ����
 		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			led0=!led0;//��ʾϵͳ��������	
			t=0;
			cnt++;
			LCD_ShowxNum(60+48,150,cnt,3,16,0X80);	//��ʾ����
		}		   
	}
}


