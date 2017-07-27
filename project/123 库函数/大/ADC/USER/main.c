#include"stm32f10x.h"
#include "adc.h"
#include "lcd1602.h"
#include "delay.h"

u8 one[10] = "ADC Value:";
u8 two[10] = "  Voltage:";

	u16 adcx,ge,shi,bai,qian;	

	u16 yi,er,san,si;

	float v;
	
void AdcValue(void);
void Voltage(void);

int main(void)
{
	u8 i ;
	
	SystemInit();
	delay_init();
	LcdInit();
	AdcInit();
	LcdAny(0XC1+10,'.');
	
	LcdWriteCmd(0x80);
	for(i=0; i<10; i++)
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
		adcx = GetAdcValue(0);
		
		AdcValue();
		Voltage();

		delay_ms(200);			
	}	
}
void AdcValue(void)
{
		qian = adcx/1000;
		bai = adcx%1000/100;
		shi = adcx%100/10;
		ge = adcx%10;

		LcdAny(0x80+10,qian+0x30);
		LcdAny(0x81+10,bai+0x30);
		LcdAny(0x82+10,shi+0x30);
		LcdAny(0x83+10,ge+0x30);
}

void Voltage(void)
{
	v = (float)adcx*(3.3/4096);
	adcx = v*1000; 

		yi = adcx/1000;
		er = adcx%1000/100;
		san = adcx%100/10;
		si = adcx%10;

		LcdAny(0xc0+10,yi+0x30);
		LcdAny(0xc2+10,er+0x30);
		LcdAny(0xc3+10,san+0x30);
		LcdAny(0xc4+10,si+0x30);
}

