/************************************************************
**  嵌入式小车核心板主控制车程序
**	www.r8c.com
**  版本号：v2.0.3
**  时间  ：2015.05.30
**  作者  ：BKRC
*************************************************************/
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "init.h"
#include "led.h"		 	 
#include "test.h"
#include "djqd.h"
#include "key.h"
#include "xj.h"
#include "csb.h"
#include "hw.h"
#include "74hc595.h"
#include "bh1750.h"

#define  NUM  10 	 // 定义接收数据长度

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

u16 CodedDisk=0;   //码盘值统计
u16 tempMP=0;	   //接收码盘值
u16 MP;			   //控制码盘值
int Car_Spend = 70;//小车速度默认值
int count = 0;	   //计数
int LSpeed;		   //循迹左轮速度
int RSpeed;		   //循迹右轮速度
u8 Line_Flag=0;

unsigned Light=0; //光照度

static u8 H_S[4]={0x80,0x7F,0x05,~(0x05)};	 //照片上翻
static u8 H_X[4]={0x80,0x7F,0x1B,~(0x1B)};	 //照片下翻

static u8 H_1[4]={0x00,0xFF,0x0C,~(0x0C)};	 //光源档位加1
static u8 H_2[4]={0x00,0xFF,0x18,~(0x18)};	 //光源档位加2
static u8 H_3[4]={0x00,0xFF,0x5E,~(0x5E)};	 //光源档位加3

static u8 H_SD[4]={0x00,0xFF,0x45,~(0x45)};	  //隧道风扇系统打开

u8 HW_K[6]={0x03,0x05,0x14,0x45,0xDE,0x92};  //报警器打开
u8 HW_G[6]={0x67,0x34,0x78,0xA2,0xFD,0x27};	//报警器关闭

static u8 CP_G1[6]={0xFF,0x12,0x01,0x00,0x00,0x00};
static u8 CP_G2[6]={0xFF,0x13,0x01,0x00,0x00,0x00};

// 主函数
int main(void)
{
	
	u8 i;
	Stm32_Clock_Init(9);        // 系统时钟设置
	delay_init(72);	            // 延时初始化 
	uart1_init(72,115200);	    // 串口初始化为115200
	uart2_init(72,115200);	    // 串口初始化为115200
	IO_Init();                  //IO初始化

	STOP();
	S_Tab[0]=0x55;
	S_Tab[1]=0xaa;

	C_Tab[0]=0x55;
	C_Tab[1]=0x02;
						
	while(1)
	{	 	   
		Track();
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
	beep=1;
}
