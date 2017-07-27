#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "can.h"

//mode����
// CAN_Mode_Normal             ((uint8_t)0x00)  /*!< normal mode */
// CAN_Mode_LoopBack           ((uint8_t)0x01)  /*!< loopback mode */
// CAN_Mode_Silent             ((uint8_t)0x02)  /*!< silent mode */
// CAN_Mode_Silent_LoopBack    ((uint8_t)0x03)  /*!< loopback combined with silent mode */

int main(void)
{
	u8 mode = CAN_Mode_LoopBack; //��ʼ��Ϊ�ػ�ģʽ
	u8 count = 0;
	u8 key;
	u8 len = 0;
	u8 i,t = 0;
	u8 res;
	u8 CanBuf[8];
	
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	uart_init(115200);
	LED_Init();
	LCD_Init();
	KEY_Init();
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_9tq,CAN_BS2_8tq,4,mode);
	
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"CAN TEST");	
	LCD_ShowString(60,90,200,16,16,"WeiHai College");
	LCD_ShowString(60,110,200,16,16,"2016/9/19");
	LCD_ShowString(60,130,200,16,16,"LoopBack Mode");	 
	LCD_ShowString(60,150,200,16,16,"KEY0:Send WK_UP:Mode");//��ʾ��ʾ��Ϣ		
	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
	LCD_ShowString(60,170,200,16,16,"Count:");			//��ʾ��ǰ����ֵ	
	LCD_ShowString(60,190,200,16,16,"Send Data:");		//��ʾ���͵�����	
	LCD_ShowString(60,250,200,16,16,"Receive Data:");	//��ʾ���յ�������		
	
	while(1)
	{
		key = KEY_Scan(0);
		if(key == KEY0_PRES)
		{
			for(i=0; i<8; i++)
			{
				CanBuf[i] = count+i;	//��䷢�ͻ�����
				if(i<4)
					LCD_ShowxNum(60+i*32,210,CanBuf[i],3,16,0x80);
				else 
					LCD_ShowxNum(60+(i-4)*32,230,CanBuf[i],3,16,0x80);
			}
			res = CAN_Send_Msg(CanBuf,8);
			if(res)
				LCD_ShowString(60+80,190,200,16,16,"Failed");		//��ʾ����ʧ��
			else 
				LCD_ShowString(60+80,190,200,16,16,"OK    ");	 		//��ʾ���ͳɹ�	
		}else if(key == WKUP_PRES)
		{
			mode = !mode;
			CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_9tq,CAN_BS2_8tq,4,mode);
			POINT_COLOR=RED;//��������Ϊ��ɫ 
			if(mode==0)//��ͨģʽ����Ҫ2��������
			{
				LCD_ShowString(60,130,200,16,16,"Nnormal Mode ");	    
			}else //�ػ�ģʽ,һ��������Ϳ��Բ�����.
			{
 				LCD_ShowString(60,130,200,16,16,"LoopBack Mode");
			}
 			POINT_COLOR=BLUE;//��������Ϊ��ɫ 
		}
		
		len = CAN_Receive_Msg(CanBuf);
		if(len)
		{
			LCD_Fill(60,270,130,310,WHITE);//���֮ǰ����ʾ
 			for(i=0;i<len;i++)
			{									    
				if(i<4)
					LCD_ShowxNum(60+i*32,270,CanBuf[i],3,16,0X80);	//��ʾ����
				else 
					LCD_ShowxNum(60+(i-4)*32,290,CanBuf[i],3,16,0X80);	//��ʾ����
 			}
		}
		t++;
		if(t == 20)
		{
			count++;
			t = 0;
			LED0 = !LED0;
			LCD_ShowxNum(60+48,170,count,3,16,0X80);	//��ʾ����
		}
		delay_ms(10);
	}
}

