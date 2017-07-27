#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "mmc_sd.h"
#include "malloc.h"	
#include "exfuns.h"
#include "fattester.h"

u8 sta;	//״̬
u16 t=0;
u32 size1,size2;  //������  ʣ������

#define DIRCT  "0:/message.h"

__inline void Card_Info(void);

int main(void)
{
	SystemInit();			//ϵͳ��ʼ�� �ɲ�д �ڻ�������ļ��� main����֮ǰ��ִ��
	delay_init();			//��ʱ��ʼ��
	uart_init(115200);		//����1��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ���2 ��λ��ռ ��λ��Ӧ
	LCD_Init();
	LED_Init();
	KEY_Init();
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	
	POINT_COLOR=RED;//��������Ϊ��ɫ	
	LCD_ShowString(20,50,200,16,16,"STM32 SPI&SD&FAT");	
	LCD_ShowString(20,70,200,16,16,"SD Card ");	
	LCD_ShowString(20,90,100,16,16,"2016-12-22");	
	
	for(t=0; t<sizeof(buf); t++)			//���Ҫд��SD�������黺��������
	{
		buf[t] = t;
		LCD_ShowxNum(10,10,t,3,16,0);
	}
	
	sta = exfuns_init();
	while(sta);
	
	sta = f_mount(fs[0],"0:",1); 					//����SD��
	while(sta);

    while(exf_getfree("0",&size1,&size2))		//��ȡ����	
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Fatfs Error!");
		delay_ms(200);
		LCD_Fill(30,150,240,150+16,WHITE);			  
		delay_ms(200);
		LED0=!LED0;
	}

	Card_Info();
	
	mf_setlabel("0:Atlandis");		//���þ��
	
	while(1)
	{
		sta = 1; 
		
		//����KEY0 ��ʽ��
		if(!KEY0)
		{
			delay_ms(10);
			if(!KEY0)
			{
				do 
				{
					LED0 = 0;                                                    //���� ��ʾ���ڸ�ʽ��
				}while(mf_fmkfs("0:",1,4096));                                  
                 LED0 = 1;                                                       //���� ��ʽ�����
				LCD_ShowString(20,190,200,16,16,"Format Successful !");
			}
		}
		//����KEY1 ��ȡ�ļ�
		if(!KEY1)
		{
			delay_ms(10);
			if(!KEY1)
			{
				sta = mf_open("0:/message.h", FA_READ);
				sta = mf_read(sizeof(buf));
				mf_close();
				if(!sta)
					LCD_ShowString(20,210,200,16,16,"File Read Successful !");
			}while(!KEY1);
		}
		//����KEY2 д���ļ�
		if(!KEY2)
		{
			delay_ms(10);
			if(!KEY2)
			{
				sta = mf_open(DIRCT, FA_CREATE_ALWAYS|FA_WRITE);
				sta = mf_write(buf,sizeof(buf));
				mf_close();
				if(!sta)
					LCD_ShowString(20,230,200,16,16,"File Write Successful !");
			}while(!KEY2);
		}
	}
}

void Card_Info(void)
{	
	LCD_ShowString(20,110,200,16,16,"SD Card check successful !!!");
	LCD_ShowString(20,130,200,16,16,"The Card type is SDHC ");
	LCD_ShowString(20,150,200,16,16,"Excess capacity:       MB");
	LCD_ShowString(20,170,200,16,16,"Remaining space:       MB");
	LCD_ShowxNum(150,150,size1>>10,5,16,0);	
	LCD_ShowxNum(150,170,size2>>10,5,16,0);	
}
