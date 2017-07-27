#include"lcd1602.h"

/*向LCD1602写命令**/
void LcdWriteCmd(u8 cmd)
{
	lcdrs0;				  //写0  选择写命令
	delay_ms(1);
	lcdrw0;				  //写0 选择写数据
	delay_ms(1);

	Data = cmd;			 //写时序
	delay_ms(1);
	lcden1;
	delay_ms(1);
	lcden0;
}
/*向LCD1602写入数据*/
void LcdWriteData(u8 dat)
{
	lcdrs1;				//写1 选择写数据
	delay_ms(1);
	lcdrw0;				//写0  选择写数据
	delay_ms(1);

	Data = dat;			//写时序
	delay_ms(1);
	lcden1;
	delay_ms(1);
	lcden0;
}
/* I/O 口 初始化函数*/
void LcdIoInit(void)
{
	RCC -> APB2ENR |= 1<<5;				  //使能GPIOD时钟
	RCC -> APB2ENR |= 1<<6;				  //使能GPIOE时钟

	GPIOD -> CRL &= 0X00000000;
	GPIOD -> CRL |= 0X77777777;
	GPIOD -> ODR |= 0XFFFF;

	GPIOE -> CRL &= 0XFFFFF000;
	GPIOE -> CRL |= 0X00000777;
	GPIOE -> ODR |= 0X0007;
}
/* LCD1602初始化函数 */
void LcdInit(void)
{
	 lcden0;
	 lcdrs0;
	 lcdrw0;

	LcdWriteCmd(0x38);	  //设置 16*2显示方式  5*7点阵 8位数据接口
	LcdWriteCmd(0x0c);	  //开显示 不显示光标
	LcdWriteCmd(0x06);	  //写一个字符 指针自动加一
	LcdWriteCmd(0x01);	  //清屏
}

void display(u8 addr,u8 dat)
{
	LcdWriteCmd(addr);		//调用写命令函数
	LcdWriteData(dat);		//调用写数据函数
}

