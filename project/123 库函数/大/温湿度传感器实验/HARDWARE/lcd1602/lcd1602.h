#ifndef _LCD1602_H_
#define _LCD1602_H_

#include "sys.h"

#define lcden0 GPIO_ResetBits(GPIOC,GPIO_Pin_2);
#define lcden1 GPIO_SetBits(GPIOC,GPIO_Pin_2);
#define lcdrw0 GPIO_ResetBits(GPIOC,GPIO_Pin_1);
#define lcdrw1 GPIO_SetBits(GPIOC,GPIO_Pin_1);
#define lcdrs0 GPIO_ResetBits(GPIOC,GPIO_Pin_0);
#define lcdrs1 GPIO_SetBits(GPIOC,GPIO_Pin_0);

#define Data GPIOD -> ODR	 /*将并行数据口宏定义为 Data*/

void LcdWriteCmd(u8 cmd);		  //写命令函数
void LcdWriteData(u8 dat);		  //写数据函数
void LcdIoInit(void);			  //I/O初始化函数
void LcdInit(void);				  //lcd初始化函数
void display(u8 addr,u8 dat);	  //自定义位置写数据函数

#endif
