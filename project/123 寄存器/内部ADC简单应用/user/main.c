#include"lcd1602.h"
#include"adc.h"

void xianshi(void);
void AdcValue(void);
void Voltage(void);
void kun(void);
					
void kun(void);

	u16 adcx,ge,shi,bai,qian;	

	u16 yi,er,san,si;

	float v;

int main(void)
{

	Stm32_Clock_Init(9);
	delay_init(72);
	LcdIoInit();
	LcdInit();
	AdcInit();
	display(0xc1,'.');

	while(1)
	{
		adcx = AdcGet(ADC_CH0);

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

		display(0x80,qian+0x30);
		display(0x81,bai+0x30);
		display(0x82,shi+0x30);
		display(0x83,ge+0x30);
}

void Voltage(void)
{
	v = (float)adcx*(3.3/4096);
	adcx = v*1000; 

		yi = adcx/1000;
		er = adcx%1000/100;
		san = adcx%100/10;
		si = adcx%10;

		display(0xc0,yi+0x30);
		display(0xc2,er+0x30);
		display(0xc3,san+0x30);
		display(0xc4,si+0x30);
}


