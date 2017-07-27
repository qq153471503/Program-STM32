#include"stm32f10x.h"
#include "lcd1602.h"
#include "delay.h"

u8 one[] = "I Love Music";
u8 two[] = "I Love EDA";

int main(void)
{
	u8 i ;
	
	SystemInit();
	delay_init();
	LcdInit();
	
	LcdWriteCmd(0X80);
	for(i=0; i<12; i++)
	{
		LcdWriteData(one[i]);
	}
	
	LcdWriteCmd(0XC0);
	for(i=0; i<10; i++)
	{
		LcdWriteData(two[i]);
	}
	while(1);	
}

