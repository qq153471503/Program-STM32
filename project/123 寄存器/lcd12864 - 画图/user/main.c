#include "lcd12864.h"
#include "delay.h"
#include "sys.h"
#include "pic.h"


u8 table[12] = "威海职业学院";
u8 table1[4] = "D545";
u8 table2[16] = "学号:20150203039";
u8 table3[12] = "姓名：刘安坤";

void StartDis(void);

int main(void)
{

	Stm32_Clock_Init(9);			  /*系统时钟初始化*/
	delay_init(72);				  /*延时初始化*/
	LcdInit();					  /*12864初始化*/

	while(1)
	{
		StartDis();
	}
}

void StartDis(void)
{
	LcdDisPic(&Photo1[0]);		  /*显示开始欢迎界面*/
	delay_ms(1000);				  /*延时显示3秒*/
	delay_ms(1000);				  //72M下 delay_ms();括号内形式参数 <= 1864 
	delay_ms(1000);				  //所以写了3个延时一秒的延时
	LcdDisPic(&Photo2[0]);		  /*显示必胜画面*/
	delay_ms(1000);				  /*延时显示3秒*/
	delay_ms(1000);				  //72M下 delay_ms();括号内形式参数 <= 1864 
	delay_ms(1000);				  //所以写了3个延时一秒的延时
}

