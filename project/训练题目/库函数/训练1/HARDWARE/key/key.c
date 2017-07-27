#include"key.h"
#include"LCD1602.h"
#include"delay.h"

u8 table[27] = "*abcdefghijklmnopqrstuvwxyz";

char k0 = 0;
u8 count = 0;
char i = 0;

void KeyInit(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��KEY0-->GPIOA.13,key0-->GPIOA.15  ��������
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//PE2~4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
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
		LCD_Writecmd(0xc0+(count-1));

			LCD_Writecmd(0x0f);
			delay_ms(500);
			LCD_Writecmd(0x0c);

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
			LCD_Writedata(table[i]);
			
	}
}

