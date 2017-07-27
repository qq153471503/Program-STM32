#include"lcd12864.h"

void LcdWriteCmd(u8 cmd)
{
	lcdrs0;
	lcdrw0;
	lcden0;
	LcdData = cmd;
	delayms(1);
	lcden1;
	delayms(1);
	lcden0;
}
void LcdWriteData(u8 dat)
{
	lcdrs1;
	lcdrw0;
	lcden0;
	LcdData = dat;
	delayms(1);
	lcden1;
	delayms(1);
	lcden0;
}
void LcdDisplay(u8 cmd,u8 dat)
{
	LcdWriteCmd(cmd);
	LcdWriteData(dat);
}

void LcdInit()
{
	RCC -> APB2ENR |= 1<<5;
	RCC -> APB2ENR |= 1<<6;

	GPIOD -> CRL &= 0X00000000;
	GPIOD -> CRL |= 0X77777777;
	GPIOD -> ODR |= 0XFFFF;

	GPIOE -> CRL &= 0XFFFFF000;
	GPIOE -> CRL |= 0X00000777;
	GPIOE -> ODR |= 0X0007;

	delayms(500);
	LcdWriteCmd(0x30);
	LcdWriteCmd(0x0c);	
	LcdWriteCmd(0x01);	
}
