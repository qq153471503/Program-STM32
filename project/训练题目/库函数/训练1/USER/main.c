#include"LCD1602.h"
#include"key.h"
#include"sys.h"
#include"delay.h"

u8 table1[12] = "My Name Is :";
u8 table2[16] = "****************";

int main(void)
{
	u8 i,k;

	SystemInit(); 			 //ϵͳʱ�ӳ�ʼ��
	delay_init(72);	 
	LcdInit();			     //lcd1602��ʼ��
	KeyInit();				 //������ʼ��
	
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

