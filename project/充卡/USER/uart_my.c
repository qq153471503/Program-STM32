/**************************************************************/
/*                    串口命令解析函数　　　　　　　　　　　　*/
/**************************************************************/

#include <stdint.h>
#include <string.h>
#include "stm32f10x.h"  //头文件包含
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
/*****************     串口传递给机械臂数据解析函数       *****************************/
/**************************************************************************************/

void  USART_WIFI_JX( u8 *buf )
{
	 u8 sum=0,len=0,i=0;  //初始赋值  sum
	 u16 t = 0;

	 len = buf[3];
	 for(i=0;i<len;i++)				 // 求和校验，取低八位
	 {
	 	 //sum=(sum+buf[i+3])%256;
	 	  t += buf[i+3];
	 }
		  sum =(u8)(~(t&0x0ff)) ;

	  // sum = ((u8)0xff)(~(sum));
//	   U1SendChar(sum);
	   if(sum==buf[len+3])			//校验通过
	   {
//			rx_len = buf[2];
//	   		for(i=0;i<rx_len;i++)		//数据通过串口1打包发出
//			{
//				 U1SendChar(buf[i]);
//			}
		  send_Flag=1;
	   }	  

}


void Normal_data(void)	  // 正常接收8字节控制指令
{
	u8 sum=0;

	if(Wifi_Rx_Buf[7]==0xbb)	 // 判断包尾
	 {									  
		//主指令与三位副指令左求和校验
		//注意：在求和溢出时应该对和做256取余。
		 sum=(Wifi_Rx_Buf[2]+Wifi_Rx_Buf[3]+Wifi_Rx_Buf[4]+Wifi_Rx_Buf[5])%256;
		if(sum == Wifi_Rx_Buf[6])
		{
			   Rx_Flag =1;
		}
		else Rx_Flag =0;
		}
}


void Abnormal_data(void)	  //数据异常处理
{
	u8 i,j;
	u8 sum=0;
	
	if(Wifi_Rx_num <8)			// 异常数据字节数小于8字节不做处理
	{
	   Rx_Flag =0;
	}
	else {
		for(i=0;i<=(Wifi_Rx_num -7);i++)  
		{
			if(Wifi_Rx_Buf[i]==0x55)	   // 寻找包头
			{
			   if(Wifi_Rx_Buf[i+7]==0xbb)	// 判断包尾
			   {
			   	    sum=(Wifi_Rx_Buf[i+2]+Wifi_Rx_Buf[i+3]+Wifi_Rx_Buf[i+4]+Wifi_Rx_Buf[i+5])%256;

		           if(sum==Wifi_Rx_Buf[i+6])	 // 判断求和
	              {
			         for(j=0;j<8;j++)
					 {
					 	 Wifi_Rx_Buf[j]=Wifi_Rx_Buf[j+i];	 // 数据搬移
					 }
					    Rx_Flag =1;
		          }
	  	         else Rx_Flag =0;
		       }
			}
	    }

	}
	

} 


void send_data_zigbee( u8 *s ,u8 len)   // 将数据转发给Zigbee
{

	Send_ZigbeeData_To_Fifo( s ,len);
	Rx_Flag =0;
}

void send_data_wifi(u8 *s ,u8 len)   // 将数据转发给wifi
{

	Send_WifiData_To_Fifo( s ,len);
	
}

