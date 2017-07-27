
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


void Wifi_data_Receive( u8 res)  // wifi 数据接收处理 
{
	
	rxd1_timer4_1ms=0;
	
//	U1SendChar( res);

	if(rxd1_timer4_flag==1) //  5ms时间 数据帧重新开始
	{
		 rxd1_timer4_flag=0;
		 TIM4->CR1|=0x01;     //使能定时器4
		 Wifi_Rx_num =0;
		 Wifi_Rx_Buf[Wifi_Rx_num]=res;

	}
	else if(Wifi_Rx_num < WIFI_MAX_NUM )	
	{
		  Wifi_Rx_Buf[++Wifi_Rx_num]=res;	 // 接收数据帧
	}

	else   // 数据超出最大接收数据长度时，舍去不要
	{
			;
	}

}

void Zigbee_data_Receive( u8 res)  // zigbee 数据接收处理
{
    u8 sum;

	if(Zigbee_Rx_num >0)
	{
	   Zigb_Rx_Buf[Zigbee_Rx_num]=res;
	   Zigbee_Rx_num++;
	}
	else if (res==0x55)		// 寻找包头
	{
		
	   Zigb_Rx_Buf[0]=res;
	   Zigbee_Rx_num=1;
	}
	else Zigbee_Rx_num =0;
	
   if(Zigbee_Rx_num >= Zigbee_Rx_Max)
	{

		if((Zigb_Rx_Buf[Zigbee_Rx_Max -1]==0xbb)&&(Zigbee_RxZt_flag ==0)&&(Zigb_Rx_Buf[1]!=0xfd))	 // 判断包尾	//change by ygm
		{									  
		    //主指令与三位副指令左求和校验
			//注意：在求和溢出时应该对和做256取余。
			Zigbee_Rx_num=0;	  // 计数清零
			Zigbee_RxZt_flag =0;  // 接收状态清零
			 sum=(Zigb_Rx_Buf[2]+Zigb_Rx_Buf[3]+Zigb_Rx_Buf[4]+Zigb_Rx_Buf[5])%256;
			if(sum==Zigb_Rx_Buf[6])
			{
	
					Zigbee_Rx_flag =1;	//   指令验证正确，标志位置1

					LED2=0;		//关闭蜂鸣器
			}
			else {LED1=2; Zigbee_Rx_flag =0;}
		}
		else if((Zigb_Rx_Buf[1]==0xfd)&&(Zigbee_RxZt_flag ==0))
		 {
			if(Zigb_Rx_Buf[2]>8)	
			{ 
			   Zigbee_Rx_Max = Zigb_Rx_Buf[2];
			   Zigbee_RxZt_flag =1;  // 接收状态1  接着接收数据
			 }
			 else 			 
			{		 
			   Zigbee_Rx_num=0;		// 计数清零
				if(Zigb_Rx_Buf[Zigbee_Rx_Max-1]==0xbb)
				 {
				   Zigbee_RxZt_flag =0;  // 接收状态清零
				 //  send_Flag=1;			//change by ygm
			       Zigbee_Rx_Max =8;
		           USART_WIFI_JX( Zigb_Rx_Buf ); // 解析串口数据
				 }
			}	  

			
		  }
		else if((Zigbee_RxZt_flag ==1)&&(Zigb_Rx_Buf[Zigbee_Rx_Max -1]==0xbb))
		{
			Zigbee_Rx_num=0;	  // 计数清零
			Zigbee_RxZt_flag =0;  // 接收状态清零
		//	send_Flag=1;
			Zigbee_Rx_Max =8;
		    USART_WIFI_JX( Zigb_Rx_Buf ); // 解析串口数据
		}
		
		
		else {LED1=2; Zigbee_Rx_flag =0; Zigbee_Rx_num =0;}		 //接收错误指令，打开蜂鸣器
	}

}


