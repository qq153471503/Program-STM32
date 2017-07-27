#include"LCD1602.h"
#include"key.h"
#include"sys.h"
#include"delay.h"

u8 table1[12] = "My Name Is :";
u8 table2[16] = "****************";

int main(void)
{
	u8 i,k;

	SystemInit(); 			 //系统时钟初始化
	delay_init(72);	 
	LcdInit();			     //lcd1602初始化
	KeyInit();				 //按键初始化
	
	LCD_Writecmd(0x80);
	for(i=0; i<12; i++)
	LCD_Writedata(table1[i]);

	LCD_Writecmd(0xc0);
	for(k=0; k<16; k++)
	LCD_Writedata(table2[k]);
	
	while(1)
	{
		KeyScan();
	}
}

