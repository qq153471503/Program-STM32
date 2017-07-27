
#include "uart_my.h"
#include "exit.h"
#include "led.h"
#include "usart.h"
#define __DATA_CHANNEL_C__
#include "data_channel.h"


u8 Zigbee_Rx_Max =8;
u8 Wifi_Rx_num =0;
u8 Wifi_Rx_flag=0;

u8 Zigbee_Rx_num =0;
u8 Zigbee_Rx_flag=0;
u8 Zigbee_RxZt_flag =0;


void Wifi_data_Receive( u8 res)  // wifi ���ݽ��մ��� 
{
	
	rxd1_timer4_1ms=0;
	
//	U1SendChar( res);

	if(rxd1_timer4_flag==1) //  5msʱ�� ����֡���¿�ʼ
	{
		 rxd1_timer4_flag=0;
		 TIM4->CR1|=0x01;     //ʹ�ܶ�ʱ��4
		 Wifi_Rx_num =0;
		 Wifi_Rx_Buf[Wifi_Rx_num]=res;

	}
	else if(Wifi_Rx_num < WIFI_MAX_NUM )	
	{
		  Wifi_Rx_Buf[++Wifi_Rx_num]=res;	 // ��������֡
	}

	else   // ���ݳ������������ݳ���ʱ����ȥ��Ҫ
	{
			;
	}

}

void Zigbee_data_Receive( u8 res)  // zigbee ���ݽ��մ���
{
    u8 sum;

	if(Zigbee_Rx_num >0)
	{
	   Zigb_Rx_Buf[Zigbee_Rx_num]=res;
	   Zigbee_Rx_num++;
	}
	else if (res==0x55)		// Ѱ�Ұ�ͷ
	{
		
	   Zigb_Rx_Buf[0]=res;
	   Zigbee_Rx_num=1;
	}
	else Zigbee_Rx_num =0;
	
   if(Zigbee_Rx_num >= Zigbee_Rx_Max)
	{

		if((Zigb_Rx_Buf[Zigbee_Rx_Max -1]==0xbb)&&(Zigbee_RxZt_flag ==0)&&(Zigb_Rx_Buf[1]!=0xfd))	 // �жϰ�β	//change by ygm
		{									  
		    //��ָ������λ��ָ�������У��
			//ע�⣺��������ʱӦ�öԺ���256ȡ�ࡣ
			Zigbee_Rx_num=0;	  // ��������
			Zigbee_RxZt_flag =0;  // ����״̬����
			 sum=(Zigb_Rx_Buf[2]+Zigb_Rx_Buf[3]+Zigb_Rx_Buf[4]+Zigb_Rx_Buf[5])%256;
			if(sum==Zigb_Rx_Buf[6])
			{
	
					Zigbee_Rx_flag =1;	//   ָ����֤��ȷ����־λ��1

					LED2=0;		//�رշ�����
			}
			else {LED1=2; Zigbee_Rx_flag =0;}
		}
		else if((Zigb_Rx_Buf[1]==0xfd)&&(Zigbee_RxZt_flag ==0))
		 {
			if(Zigb_Rx_Buf[2]>8)	
			{ 
			   Zigbee_Rx_Max = Zigb_Rx_Buf[2];
			   Zigbee_RxZt_flag =1;  // ����״̬1  ���Ž�������
			 }
			 else 			 
			{		 
			   Zigbee_Rx_num=0;		// ��������
				if(Zigb_Rx_Buf[Zigbee_Rx_Max-1]==0xbb)
				 {
				   Zigbee_RxZt_flag =0;  // ����״̬����
				 //  send_Flag=1;			//change by ygm
			       Zigbee_Rx_Max =8;
		           USART_WIFI_JX( Zigb_Rx_Buf ); // ������������
				 }
			}	  

			
		  }
		else if((Zigbee_RxZt_flag ==1)&&(Zigb_Rx_Buf[Zigbee_Rx_Max -1]==0xbb))
		{
			Zigbee_Rx_num=0;	  // ��������
			Zigbee_RxZt_flag =0;  // ����״̬����
		//	send_Flag=1;
			Zigbee_Rx_Max =8;
		    USART_WIFI_JX( Zigb_Rx_Buf ); // ������������
		}
		
		
		else {LED1=2; Zigbee_Rx_flag =0; Zigbee_Rx_num =0;}		 //���մ���ָ��򿪷�����
	}

}


