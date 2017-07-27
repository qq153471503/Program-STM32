/************************************************************
**  智能小车核心板控制程序
**	www.r8c.com
**  版本号：V1.1
**  时间  ：2017.03.08
**  作者  ：BKRC
**  修改记录： 增加CAN接口
**  推荐使用 参数 ： 寻迹速度 50   转弯速度  80 
*************************************************************/
#include "stm32f10x.h"  //包含
#include "sys.h"					
#include "usart.h"		
#include "delay_drv.h"
#include "init.h"
#include "led.h"		 	 
#include "exit.h"
#include "test.h"
#include "djqd.h"
#include "key.h"
#include "xj.h"
#include "csb.h"
#include "hw.h"
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
#include "bz.h"
#include "zx.h"
#include "rc522.h"

#define  NUM  10 	 // 定义接收数据长度
#define  ZCKZ_ADDR    0xAA  // 定义运输标志物地址编号
#define  YSBZW_ADDR   0x02  // 定义运输标志物地址编号
#define  DZ_ADDR      0x03  // 定义道闸标志物地址编号
#define  LEDXS_ADDR   0x04  // 定义LED显示标志物地址编号
#define  JXB_ADDR     0x05  // 定义机械臂标志物地址编号


void IO_Init(void); //IO初始化	

//u8 mp;	
u8 G_Tab[6];	   //定义红外发射数组
u8 S_Tab[NUM]; 	   //定义主返回数据数组
u8 C_Tab[NUM]; 	   //定义运动标志物返回数据数组

u8 Stop_Flag=0;    //状态标志位
u8 Track_Flag=0;   //循迹标志位
u8 G_Flag=0;	   //前进标志位
u8 B_Flag=0;	   //后退标志位
u8 L_Flag=0;	   //左转标志位
u8 R_Flag=0;	   //右转标志位
u8 SD_Flag=1;      //运动标志物数据返回允许标志位
u8 Rx_Flag =0;
unsigned int R;

u16 CodedDisk=0;   //码盘值统计
u16 tempMP=0;	   //接收码盘值
u16 MP;			   //控制码盘值
int Car_Spend = 50;//小车速度默认值
int count = 0;	   //计数
int LSpeed;		   //循迹左轮速度
int RSpeed;		   //循迹右轮速度
u8 Line_Flag=0;	   // 
u8 send_Flag=0;	   // 发送标志位
u8 ka=0,biao=1,ab,cd,xy;
u8 guang=0,ku=1;
u8 Kuai=1;
u8 anjian=1;
u8 tu1,tu2,tu3,mm=0;
u8 weizhi,fangxiang;
// 主函数

RCC_ClocksTypeDef RCC_Clocks;
u8 Card1[21]= {0x55,0xAA,0x09,0x09,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0xBB};
u8 Card2[16]=	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
u8 TabGQ[5];
u8 TabCSB[4];
static u8 GUANGDANG1[26]={"智能照明系统预设档位为一档"};	
void SysTick_Handler(void)
{
	global_times++;
	if(delay_ms_const)
		delay_ms_const--;
}


extern void pwm_test(void);

int main(void)
{
	u8 ut=0;
	u8 i;
	unsigned int a;
	global_times = 0;
	SystemInit();
  Delay_us_Init();
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	RC522_Uart_init(9600);
	InitRc522();
	RC522_Uart_init(115200);
	


	NVIC_Configuration(); 	 //设置NVIC中断分组0:2位抢占优先级，2位响应优先级
	
//	Xj_Track_Test();
	
	uart1_init(115200);	    // 串口初始化为115200

	uart2_init(115200, 0);	    // 串口初始化为115200 // 默认开中断
	
	IO_Init();                  //IO初始化

	S_Tab[0]=0x55;
	S_Tab[1]=0xaa;

	C_Tab[0]=0x55;
	C_Tab[1]=0x02;
	
	CanP_Init();
	
	Power_Check();  //电量检测  上电先检测一次电量
	Send_Electric( Electric_Buf[0],Electric_Buf[1]);
	

	STOP();
	Host_Close_UpTrack();
	
	Send_Debug_Info("A1B2C3\n",8); // 上传调试信息
 	
	while(1)
	{	 	   
		
 	  ut ++;
		LED0=!LED0;	//程序状态

//		CanP_Host_Main();      // 放到定时器中去接收数据
//		CanP_CanTx_Check();
//		
//		 SYN7318_Test();
	
		if( !(ut %100))
		{		
			Power_Check();  //电量检测  无需实时操作

      Send_Electric( Electric_Buf[0],Electric_Buf[1]);
			
						
		}
		  S_Tab[2]=Stop_Flag;   	//小车运行状态

		  if(PSS==1) S_Tab[3]=1;	//光敏状态值返回
		     else S_Tab[3]=0;

		   tran();

		   S_Tab[4]=dis%256;        //超声波数据
		   S_Tab[5]=dis/256;
		   	    
			  
		   S_Tab[8]=CodedDisk%256;  //码盘
		   S_Tab[9]=CodedDisk/256;
		

		 if(!KEY0)			  //按键K1按下
		 {
		   delay_ms(30);
		 if(!KEY0)
		 {
				Rc522(25,1);	
				Rc522(25,1);	
				Rc522(25,1);	
				WRITE_RFID[12]=0x1A;
				WRITE_RFID[13]=0xE5;
				WRITE_RFID[14]=0x1A;
				WRITE_RFID[15]=0xE5;
				Rc522(26,1);	
				Rc522(26,1);	
				Rc522(26,1);	
			    
		 }
		}
	if(!KEY1)			//按键K2按下
	{
		delay_ms(30);
		if(!KEY1)
		{	
			Rc522(25,0);
			Rc522(26,0);
		}
	}
	   if(!KEY2)			  //按键K3按下
		{
		   delay_ms(30);
		if(!KEY2)
		{
			
		}
		}
		if(!KEY3)			  //按键K4按下
		{
		  delay_ms(30);
	    if(!KEY3)
			{
				Rc522(25,0);	
			}
		}

	
		if(Wifi_Rx_flag ==1)  // wifi 接收标记
		{
			  Wifi_Rx_flag =0;

			  if(Wifi_Rx_Buf[0]==0xFD)  // 接收到的语音控制数据，直接转发
			  {
					  send_data_zigbee( Wifi_Rx_Buf , (Wifi_Rx_num +1)); 
			  }
			  else if(Wifi_Rx_Buf[0]==0x55)  // 接收到55开头数据
			  {
			  	 
			  	  Normal_data();  //正常数据处理
			  }
			  else {
			  	   Abnormal_data();      // 异常数据处理
			  }
		
		}

		if(Rx_Flag ==1)	//接收到控制指令
		{

		STOP();
		delay_us(5);
		Rx_Flag=0;	

		if(Wifi_Rx_Buf[1]==ZCKZ_ADDR) 	   //主车控制
		{
//			beep=1;
//			delay_ms(100);
//			beep=0;
				if(Wifi_Rx_Buf[2]==0x01)
				{							
					line=5;
		
					while(1)
					{
						gongzuo();
					}
					
				}
				
	    	Rx_Flag =0;
		}

		else  send_data_zigbee( Wifi_Rx_Buf ,8);			  // 非主车数据转发给其他zigbee节点

		} 
		
		if(Zigbee_Rx_flag ==1)	 //zigbee返回信息
		{
		  STOP()  ; 
	      delay_us(5);

			if( (Zigb_Rx_Buf[1]==0x03) || (Zigb_Rx_Buf[1]==0x0c)) // 道闸 或 ETC
			{
			   if(Zigb_Rx_Buf[2]==0x01)
			   {
					if(Zigb_Rx_Buf[3]==0x01)
					{
						Stop_Flag=Zigb_Rx_Buf[4] ;	  // 05 车道道闸完成  06 ETC
						Zigbee_Rx_flag =0;
					}
					else Zigbee_Rx_flag =0;			   

			   }
			    else Zigbee_Rx_flag =0;
			
			}	

			else if(Zigb_Rx_Buf[1]==0x21)	 //第一组数据
			  {

					C_Tab[2]=Zigb_Rx_Buf[2];
					C_Tab[3]=Zigb_Rx_Buf[3];
					C_Tab[4]=Zigb_Rx_Buf[4];
					C_Tab[5]=Zigb_Rx_Buf[5];
					//flag3 = 1;

				}
		     else if(Zigb_Rx_Buf[1]==0x22)	 //第二组数据
				{

					C_Tab[6]=Zigb_Rx_Buf[2];
					C_Tab[7]=Zigb_Rx_Buf[3];
					C_Tab[8]=Zigb_Rx_Buf[4];
					C_Tab[9]=Zigb_Rx_Buf[5];

					flag3 = 1;
				}
			  
			else if(Zigb_Rx_Buf[1]==0x06)
			   {
				if(Zigb_Rx_Buf[2]==0x01)
					{
						Stop_Flag=Zigb_Rx_Buf[3] ;	  // 语音芯片状态返回
						Zigbee_Rx_flag =0;
					}
					else Zigbee_Rx_flag =0;
		     	}

			   else Zigbee_Rx_flag =0;


			 Zigbee_Rx_flag =0;		
		}

		if(send_Flag==1)
		{
// 		   for(i=0;i<Zigb_Rx_Buf[2];i++)		//机械臂数据通过串口1打包发出
// 			{
// 				 U1SendChar(Zigb_Rx_Buf[i]);
// 			}
			 send_data_wifi(Zigb_Rx_Buf ,Zigb_Rx_Buf[2]);
			
		   send_Flag=0;

	    }

		else if((flag3==1)&&(SD_Flag==0))
		  {
// 		  for(i=0;i<10;i++)	 // 主车向平板电脑发送运输标志物数据
// 		    {
// 	          U1SendChar(C_Tab[i]);
// 		    }
				send_data_wifi(C_Tab , 10);
				
			flag3 = 0;
	  	   }
		else if((SD_Flag))
		   {
		 
 //		  		 send_data_wifi(S_Tab , 10);
				 
		    }
	
	}
	
}

/***************************************************************
** 功能：     初始化核心板所使用端口
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void IO_Init(void)
{
	YJ_INIT();			//初始化硬件

	GPIOB->CRH&=0XFFFFFFF0;	  
	GPIOB->CRH|=0X00000008;//PB8 设置成输入   			   
	GPIOB->ODR|=1<<8;	   //PB8上拉

	GPIOC->CRH&=0X000FFFFF; 
	GPIOC->CRH|=0X33300000;   //PC13/PC14/PC15推挽输出  	 
  GPIOC->ODR|=0XE000;       //PC13/PC14/PC15输出高
		   	 
	GPIOD->CRH&=0XFFF0FFFF; 
	GPIOD->CRH|=0X00030000;   //PD12推挽输出  	 
  GPIOD->ODR|=0X1000;       //PD12推挽高											  
	
	LED_L=1;				  
	LED_R=1;
	BEEP=1;
	
	beep=0;
		
}
