#include"key.h"
#include"lcd1602.h"

char count = 0;
char i = 0;

u8 table[27] = "*abcdefghijklmnopqrstuvwxyz";

void KeyInit(void)
{
	RCC -> APB2ENR |= 1<<2;
	RCC -> APB2ENR |= 1<<6;
	
	GPIOA -> CRL &= 0XFFFFFFF0;
	GPIOA -> CRL |= 0X00000008;
	
	GPIOE -> CRL &= 0XFFF000FF;
	GPIOE -> CRL |= 0X00088800;
	GPIOE -> ODR |= 0X001C;


}
void KeyScan(void)
{
	if(key0 == 0)
	{
		delay_ms(10);

		if(key0 == 0)
		{
			count++;
			if(count == 17)
			{
				count = 0;
				i = 0;
			}
			while(!key0);
		}
	}

	if(count != 0)
	{
		LcdWriteCmd(0xc0+(count-1));

			LcdWriteCmd(0x0f);
			delay_ms(500);
			LcdWriteCmd(0x0c);

			if(key1 == 0)
			{
				delay_ms(10);

				if(key1 == 0)
				{
					i++;
					if(i == 28)
						i = 0;
					while(!key1);
				}
			}
			LcdWriteData(table[i]);
	}
}

