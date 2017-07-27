#include "lcd12864.h"
#include "delay.h"
#include "picture.h"

/*《说明》
delay_ms(1000);				  //72M下 delay_ms();括号内形式参数 <= 1864 
12864是8位数据口 读取写入速度较慢，开机后需等待一段时间才能看到图画*/


int main(void)
{
	SystemInit();
	delay_init();
	LcdInit();
	LcdCleanScreen();				/*清屏*/
	
	LcdShowPic(&Photo1[0]);		  /*显示开始欢迎界面*/
}

