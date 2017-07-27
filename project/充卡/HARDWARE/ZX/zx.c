#include "stm32f10x.h"  //°üº¬
#include "sys.h"					
#include "usart.h"		
#include "delay_drv.h"
#include "init.h"	 	 
#include "test.h"
#include "xj.h"
#include "74hc595.h"
#include "bh1750.h"
#include "uart_my.h"
#include "canp_hostcom.h"
#include "can_drv.h"
#include "fifo_drv.h"
#include "data_channel.h"
#include "power_check.h"
#include "syn7318.h"
#include "iic.h"
#include "hmc5883l.h"
#include "zx.h"
#include "bz.h"
#include "rc522.h"
#include "hw.h"
#include "exit.h"

u8 line;

void gongzuo(void)
{
	u8 i;
	switch(line)
	{
		
		case 5 :
						OpenJS();
						xunji();		
						qianjingMP(15);
						qianjingMP(45);
						erweima();
						line=10;
						break;
		case 10:
						xunji();
						qianjingMP(15);
						line=15 ;
						break;
		
		case 15:
						xunji();
						qianjingMP(15);
						
						line=20;
						break;
		case 20:
						xunji();
						CLight();
						houtuiMP(40);
						Byouzhuan();
						line=25;
						break;
		case 25:
						ka=1;
						xunji();
						qianjingMP(15);
						line=30;
						break;
		case 30:	
						
						xunji();
						qianjingMP(15);
						ka=0;
						Chepai();
						Byouzhuan();
						line=35;
						break;
		case 35:
						xunji();
						qianjingMP(3);
						youzhuan();
						qianjingMP(5);
						LED_L=1;LED_R=1;
						sishiwu();
						OpenBJQ();
						Bzuozhuan();
						TFTOpenJS();
						delay_ms(1000);
						congche();
						ETCopen();
						line=40;
						break;
		case 40:
						xunji();
						zuozhuan();
						line=45;
						break;
		case 45:
						xunji();
						zuozhuan();
						qianjingMP(55);
						Tuxing();
						houtuiMP(50);
						Bzuozhuan();
						xunji();
						zuozhuan();
						Bzuozhuan();
						xunji();
						qianjingMP(15);
						SYN7318_Open();   
						SYN7318_Test();
						line=50;
						break;
		case 46:
						zuozhuan();
						xunji();
						zuozhuan();
						xunji();
						youzhuan();
						xunji();
						qianjingMP(20);
						CloseJS();
						delay_ms(100);
						BEEP=0;
						delay_ms(1500);
						BEEP=1;
						Cixuanfu();
						while(1);
						break;
		case 47:
						youzhuan();
						xunji();
						youzhuan();
						xunji();
						youzhuan();
						xunji();
						qianjingMP(15);
						xunji();
						qianjingMP(15);
						xunji();
						qianjingMP(20);
						CloseJS();
						delay_ms(100);
						BEEP=0;
						delay_ms(1500);
						BEEP=1;
						Cixuanfu();
						while(1);
						break;
		case 48:
						LED_L=0;LED_R=0;
						zuozhuan();
						Bzuozhuan();
						LED_L=1;LED_R=1;
						xunji();
						youzhuan();
						xunji();
						qianjingMP(15);
						xunji();
						qianjingMP(20);
						CloseJS();
						delay_ms(100);
						BEEP=0;
						delay_ms(1500);
						BEEP=1;
						Cixuanfu();
						while(1);
						break;
		case 50:
						Zigbee_Rx_flag=0;
						Zigb_Rx_Buf[0]=0x55;
						Zigb_Rx_Buf[1]=0x02;
						Zigb_Rx_Buf[2]=0x01;
						Zigb_Rx_Buf[3]=tu2;
						Zigb_Rx_Buf[4]=0x00;
						Zigb_Rx_Buf[5]=0x00;
						Zigb_Rx_Buf[6]=(0x01+tu2%0xFF);
						Zigb_Rx_Buf[7]=0xBB;
						for(i=0;i<3;i++)
						ZigbeeSend();	
						delay_ms(1000);
						delay_ms(1000);
						Zigbee_Rx_flag=0;
						while(Zigbee_Rx_flag==0);
						CloseJS();
						line=0;
						break;
	}	
}
