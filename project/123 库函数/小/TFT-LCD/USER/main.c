/*��ʾͼƬ���ߴ� 200*230 = 46000����

���� ͼƬ�����������Ϊ92000��*/

#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "pic.h"/*ͼƬ����ͷ�ļ�*/

int main(void)
{
	u8 x = 0;

	SystemInit();
	uart_init(9600);
	delay_init();
	LedInit();
	LcdInit();

	LcdClearScreen(RED);
	  	
	while(1)/*�����������ʾ*/
	{
		switch(x)/*������ɫ�任*/
		{
			case 0:LcdClearScreen(WHITE);break;
			case 1:LcdClearScreen(BLACK);break;
			case 2:LcdClearScreen(BLUE);break;
			case 3:LcdClearScreen(RED);break;
			case 4:LcdClearScreen(MAGENTA);break;
			case 5:LcdClearScreen(GREEN);break;
			case 6:LcdClearScreen(CYAN);break;
			case 7:LcdClearScreen(YELLOW);break;
			case 8:LcdClearScreen(BRRED);break;
			case 9:LcdClearScreen(GRAY);break;
			case 10:LcdClearScreen(LGRAY);break;
			case 11:LcdClearScreen(BROWN);break;
		}
//		PointColor = RED;
		
			LcdDrawPicture(40,30,240,180);
//				
//		LcdShowString(60,90,"WeiHai College");
//		LcdShowString(60,110,"2016/08/01");
//		LcdShowString(60,130,"20:07:32  PM");
//		LcdShowString(0,304,"Menu");
//		LcdShowString(192,304,"Return");
		
	    x++;
		
		if( x == 12 )
			x = 0;

//		led0 = ~led0;
//		delay_ms(1000);
//	}

//	while(1)/*����Ǻ�����ʾ*/
//	{
//		switch(x)/*������ɫ�任*/
//		{
//			case 0:LcdClearScreen(WHITE);break;
//			case 1:LcdClearScreen(BLACK);break;
//			case 2:LcdClearScreen(BLUE);break;
//			case 3:LcdClearScreen(RED);break;
//			case 4:LcdClearScreen(MAGENTA);break;
//			case 5:LcdClearScreen(GREEN);break;
//			case 6:LcdClearScreen(CYAN);break;
//			case 7:LcdClearScreen(YELLOW);break;
//			case 8:LcdClearScreen(BRRED);break;
//			case 9:LcdClearScreen(GRAY);break;
//			case 10:LcdClearScreen(LGRAY);break;
//			case 11:LcdClearScreen(BROWN);break;
//		}
//		PointColor = RED;
//				
//		LcdShowString(100,50,"WeiHai College");
//		LcdShowString(100,70,"2016/08/01");
//		LcdShowString(100,90,"20:07:32  PM");
//		LcdShowString(2,222,"Menu");
//		LcdShowString(270,222,"Return");
//	    x++;
//		
//		if( x == 12 )
//			x = 0;

		led0 = ~led0;
		delay_ms(1000);
//	}
	}
}
