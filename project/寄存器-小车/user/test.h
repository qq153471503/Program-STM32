#ifndef __TEST_H
#define __TEST_H	 
#include "sys.h"


#define LED_L PCout(15) //PC15 任务板左灯定义
#define LED_R PCout(14) //PC14 任务板右灯定义
#define BEEP PCout(13)  //PC13 任务板蜂鸣器定义
#define beep PDout(12)  //PD12 核心板蜂鸣器定义
#define PSS PBin(8)     //PB8  任务板光敏状态定义

extern u8 Stop_Flag; 
extern u8 Track_Flag;
extern u8 G_Flag;
extern u8 B_Flag;
extern u8 L_Flag;
extern u8 R_Flag;
extern u16 NowMP;
extern u16 CodedDisk;
extern u16 tempMP;
extern u16 MP;
extern int Car_Spend;		 //循迹速度
extern int count;
extern int corSpeed;		//循迹调节速度
extern int LSpeed;		   //循迹左轮速度
extern int RSpeed;		   //循迹右轮速度
extern u8 Line_Flag;


extern u8 HW_K[6];  //报警器打开
extern u8 HW_G[6];	//报警器关闭
	 				    
#endif


