#include"lcd1602.h"
#include "delay.h"

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
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
}
/* LCD1602初始化函数 */
void LcdInit(void)
{
	 lcden0;
	delay_us(10);
	 lcdrs0;
	delay_us(10);
	 lcdrw0;
	delay_us(10);

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

