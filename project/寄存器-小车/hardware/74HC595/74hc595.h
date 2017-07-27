#ifndef __74HC595_H
#define __74HC595_H	
#include "sys.h"


//74HC595�������Ŷ��� 
#define OE   PCout(0) //PC0	 
#define RCLK PCout(1) //PC1	
#define SCLK PCout(2) //PC2
#define SER  PCout(3) //PC3


extern void HC595_Init(void); //��ʼ��
extern void Write_595(u8 Data); 
		 				    
#endif	

