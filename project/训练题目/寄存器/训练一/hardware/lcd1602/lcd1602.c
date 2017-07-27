#include"lcd1602.h"

void LcdWriteCmd(u8 cmd)
{
	lcdrs0;
	delay_ms(1);
	lcdrw0;
	delay_ms(1);

	Data = cmd;
	delay_ms(1);
	lcden1;
	delay_ms(1);
	lcden0;
}
void LcdWriteData(u8 dat)
{
	lcdrs1;
	delay_ms(1);
	lcdrw0;
	delay_ms(1);

	Data = dat;
	delay_ms(1);
	lcden1;
	delay_ms(1);
	lcden0;
}

void LcdInit(void)
{
	RCC -> APB2ENR |= 1<<4;
	RCC -> APB2ENR |= 1<<5;

	GPIOD -> CRL &= 0X00000000;
	GPIOD -> CRL |= 0X77777777;
	GPIOD -> ODR |= 0XFFFF;

	GPIOC -> CRL &= 0XFFFFF000;
	GPIOC -> CRL |= 0X00000777;
	GPIOC -> ODR |= 0X0007;

	delay_ms(100);

	 lcden0;
	 lcdrs0;
	 lcdrw0;

	LcdWriteCmd(0x38);
	LcdWriteCmd(0x0c);
	LcdWriteCmd(0x06);
	LcdWriteCmd(0x01);
}

void LcdWritwChar(u8 cmd,u8 dat)
{
	LcdWriteCmd(cmd);
	LcdWriteData(dat);
}

