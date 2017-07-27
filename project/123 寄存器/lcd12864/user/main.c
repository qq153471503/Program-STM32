#include"lcd12864.h"
#include"delay.h"
#include"sys.h"

//void display(void);
//
//	u8 i = 0x80;
//	u8 k = 0;

u8 table[12] = "威海职业学院";
u8 table1[4] = "D545";
u8 table2[16] = "学号:20150203039";
u8 table3[12] = "姓名：刘安坤";

int main(void)
{
	u8 i ;

	Stm32ClockInit(9);
	DelayInit(72);
	LcdInit();

	while(1)
	{
		LcdWriteCmd(0x81);
		for(i=0; i<12; i++)
		{
			LcdWriteData(table[i]);
		}

		LcdWriteCmd(0x93);
		for(i=0; i<4; i++)
		{
			LcdWriteData(table1[i]);
		}
		
		LcdWriteCmd(0x88);
		for(i=0; i<16; i++)
		{
			LcdWriteData(table2[i]);
		}

		LcdWriteCmd(0x98);
		for(i=0; i<12; i++)
		{
			LcdWriteData(table3[i]);
		}		
	}
}

//void display(void)
//{
//	LcdDisplay(0x80,table[0]);
//	LcdDisplay(0x81,table[1]);
//	LcdDisplay(0x82,table[2]);
//	LcdDisplay(0x83,table[3]);
//	LcdDisplay(0x84,table[4]);
//	LcdDisplay(0x85,table[5]);
//	LcdDisplay(0x86,table[6]);
//	LcdDisplay(0x87,table[7]);
//	LcdDisplay(0x88,table[8]);
//	LcdDisplay(0x89,table[9]);
//	LcdDisplay(0x8A,table[10]);
//	LcdDisplay(0x8B,table[11]);
//}
