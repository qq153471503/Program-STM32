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
	//初始化KEY0-->GPIOA.13,key0-->GPIOA.15  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//PE2~4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
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

