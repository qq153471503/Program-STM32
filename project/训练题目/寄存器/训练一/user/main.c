#include"lcd1602.h"
#include"key.h"

u8 one[12] = {"My Name is :"};
u8 two[16] = {"****************"};

int main(void)
{
	u8 z,x;

	Stm32_Clock_Init(9);
	delay_init(72);
	LcdInit();
	KeyInit();

	LcdWriteCmd(0x80);
	for(z=0; z<12; z++)
		LcdWriteData(one[z]);
	LcdWriteCmd(0xc0);
	for(x=0; x<16; x++)
		LcdWriteData(two[x]);

	while(1)
	{
		KeyScan();
	}
}
