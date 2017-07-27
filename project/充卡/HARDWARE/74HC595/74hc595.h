#ifndef __74HC595_H
#define __74HC595_H	
#include "sys.h"


//74HC595控制引脚定义 
#define OE   PDout(7) //PD7 
#define RCLK PDout(6) //PD6	
#define SCLK PDout(5) //PD5
#define SER  PDout(4) //PD4


extern void HC595_Init(void); //初始化
extern void Write_595(u8 Data); 
extern void test_hc595( void );		 				    
#endif	

