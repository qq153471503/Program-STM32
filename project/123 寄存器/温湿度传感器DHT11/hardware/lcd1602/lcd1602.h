#ifndef _LCD1602_H_
#define _LCD1602_H_

#include"sys.h"
#include"delay.h"

//使能 置0
#define lcden0 GPIOE -> BRR = 0x0004			//lcd使能 PE2
//使能 置1
#define lcden1 GPIOE -> BSRR = 0X0004

#define lcdrs0 GPIOE -> BRR = 0X0001			// PE1
#define lcdrs1 GPIOE -> BSRR = 0X0001

#define lcdrw0 GPIOE -> BRR = 0X0002			//PE0
#define lcdrw1 GPIOE -> BSRR = 0X0002

#define Data GPIOD -> ODR	 /*将并行数据口宏定义为 Data*/

void LcdWriteCmd(u8 cmd);		  //写命令函数
void LcdWriteData(u8 dat);		  //写数据函数
void LcdIoInit(void);			  //I/O初始化函数
void LcdInit(void);				  //lcd初始化函数
void display(u8 addr,u8 dat);	  //自定义位置写数据函数

#endif
