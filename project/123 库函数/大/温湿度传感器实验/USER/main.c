/************************************************************************/
/*							威海职业学院								*/
/*						电子创新协会	D545实验室						*/
/*							Date: 2016/7/12								*/
/*								 Liu									*/
/*                       												*/
/*实验名称：温湿度传感器的应用											*/
/*功能介绍：温湿度传感器检测环境温湿度，将数据显示到lcd1602显示屏上。	*/
/*DHT11温湿度传感器简介： 可测空气湿度：0%-100%							*/
/*						  可测空气温度：0℃-50℃						*/
/************************************************************************/

					#include"delay.h"
					#include"lcd1602.h"
					#include"dht11.h"
					
u8 one[] = " Humi Now:   %";
u8 two[] = " Temp Now:";

/*该函数显示出温湿度外的字符*/
void DisInit(void);	 

int main(void)
{
	u8 i ;
	
	SystemInit();	  //系统时钟初始化
	delay_init();			  //延时初始化
	LcdIoInit();
	LcdInit();
	DisInit();
	Dht11IoInit();
	
	LcdWriteCmd(0x80);
	for(i=0; i<14; i++)
	{
		LcdWriteData(one[i]);
	}
	LcdWriteCmd(0xc0);
	for(i=0; i<10; i++)
	{
		LcdWriteData(two[i]);
	}

	while(1)
	{
		ReceiveData();		//调用读取温湿度函数

		display(0x80+11,humih/10+0X30);		//将数据分解个位 十位  
		display(0x80+12,humih%10+0X30);		//+0x30 表示将数据变成10进制显示

		display(0xC0+11,temph/10+0X30);
		display(0xC0+12,temph%10+0X30);
		
		delay_ms(120);		   //每120ms执行一次读写
	}
}

/*此函数用于显示摄氏度的单位符号*/
void DisInit()
{
	display(0xc0+13,0xdf);
	display(0xc0+14,0x43);
}
