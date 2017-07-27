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

/*该函数显示出温湿度外的字符*/
void DisInit(void);	 

int main(void)
{
	Stm32_Clock_Init(9);	  //系统时钟初始化
	delay_init(72);			  //延时初始化
	LcdIoInit();
	LcdInit();
	DisInit();
	Dht11IoInit();

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

void DisInit()
{
	display(0x80+1,'H');
	display(0x80+2,'u');
	display(0x80+3,'m');
	display(0x80+4,'i');
	display(0x80+5,' ');
	display(0x80+6,'N');
	display(0x80+7,'o');
	display(0x80+8,'w');
	display(0x80+9,':');
	display(0x80+13,'%');

	display(0xc0+1,'T');
	display(0xc0+2,'e');
	display(0xc0+3,'m');
	display(0xc0+4,'p');
	display(0xc0+5,' ');
	display(0xc0+6,'N');
	display(0xc0+7,'o');
	display(0xc0+8,'w');
	display(0xc0+9,':');
	display(0xc0+13,0xdf);
	display(0xc0+14,0x43);
}
