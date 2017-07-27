/**************************************************************/
/*                    ���������������������������������������*/
/**************************************************************/

#include <stdint.h>
#include <string.h>
#include "stm32f10x.h"  //ͷ�ļ�����
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h" 	 
#include "test.h"
#include "data_channel.h"
#include "canp_hostcom.h"
#include "can_drv.h"
#include "fifo_drv.h"


#define __UART_MY_C__
#include "uart_my.h"


/***************************************************************************************/
/*****************     ���ڴ��ݸ���е�����ݽ�������       *****************************/
/**************************************************************************************/

void  USART_WIFI_JX( u8 *buf )
{
	 u8 sum=0,len=0,i=0;  //��ʼ��ֵ  sum
	 u16 t = 0;

	 len = buf[3];
	 for(i=0;i<len;i++)				 // ���У�飬ȡ�Ͱ�λ
	 {
	 	 //sum=(sum+buf[i+3])%256;
	 	  t += buf[i+3];
	 }
		  sum =(u8)(~(t&0x0ff)) ;

	  // sum = ((u8)0xff)(~(sum));
//	   U1SendChar(sum);
	   if(sum==buf[len+3])			//У��ͨ��
	   {
//			rx_len = buf[2];
//	   		for(i=0;i<rx_len;i++)		//����ͨ������1�������
//			{
//				 U1SendChar(buf[i]);
//			}
		  send_Flag=1;
	   }	  

}


void Normal_data(void)	  // ��������8�ֽڿ���ָ��
{
	u8 sum=0;

	if(Wifi_Rx_Buf[7]==0xbb)	 // �жϰ�β
	 {									  
		//��ָ������λ��ָ�������У��
		//ע�⣺��������ʱӦ�öԺ���256ȡ�ࡣ
		 sum=(Wifi_Rx_Buf[2]+Wifi_Rx_Buf[3]+Wifi_Rx_Buf[4]+Wifi_Rx_Buf[5])%256;
		if(sum == Wifi_Rx_Buf[6])
		{
			   Rx_Flag =1;
		}
		else Rx_Flag =0;
		}
}


void Abnormal_data(void)	  //�����쳣����
{
	u8 i,j;
	u8 sum=0;
	
	if(Wifi_Rx_num <8)			// �쳣�����ֽ���С��8�ֽڲ�������
	{
	   Rx_Flag =0;
	}
	else {
		for(i=0;i<=(Wifi_Rx_num -7);i++)  
		{
			if(Wifi_Rx_Buf[i]==0x55)	   // Ѱ�Ұ�ͷ
			{
			   if(Wifi_Rx_Buf[i+7]==0xbb)	// �жϰ�β
			   {
			   	    sum=(Wifi_Rx_Buf[i+2]+Wifi_Rx_Buf[i+3]+Wifi_Rx_Buf[i+4]+Wifi_Rx_Buf[i+5])%256;

		           if(sum==Wifi_Rx_Buf[i+6])	 // �ж����
	              {
			         for(j=0;j<8;j++)
					 {
					 	 Wifi_Rx_Buf[j]=Wifi_Rx_Buf[j+i];	 // ���ݰ���
					 }
					    Rx_Flag =1;
		          }
	  	         else Rx_Flag =0;
		       }
			}
	    }

	}
	

} 


void send_data_zigbee( u8 *s ,u8 len)   // ������ת����Zigbee
{

	Send_ZigbeeData_To_Fifo( s ,len);
	Rx_Flag =0;
}

void send_data_wifi(u8 *s ,u8 len)   // ������ת����wifi
{

	Send_WifiData_To_Fifo( s ,len);
	
}

