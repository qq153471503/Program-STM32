#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"

//蜂鸣器端口定义
#define beep PBout(8)	// BEEP,蜂鸣器接口		   

void BeepInit(void);	//初始化
		 				    
#endif

