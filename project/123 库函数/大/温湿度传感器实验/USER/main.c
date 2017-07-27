/************************************************************************/
/*							����ְҵѧԺ								*/
/*						���Ӵ���Э��	D545ʵ����						*/
/*							Date: 2016/7/12								*/
/*								 Liu									*/
/*                       												*/
/*ʵ�����ƣ���ʪ�ȴ�������Ӧ��											*/
/*���ܽ��ܣ���ʪ�ȴ�������⻷����ʪ�ȣ���������ʾ��lcd1602��ʾ���ϡ�	*/
/*DHT11��ʪ�ȴ�������飺 �ɲ����ʪ�ȣ�0%-100%							*/
/*						  �ɲ�����¶ȣ�0��-50��						*/
/************************************************************************/

					#include"delay.h"
					#include"lcd1602.h"
					#include"dht11.h"
					
u8 one[] = " Humi Now:   %";
u8 two[] = " Temp Now:";

/*�ú�����ʾ����ʪ������ַ�*/
void DisInit(void);	 

int main(void)
{
	u8 i ;
	
	SystemInit();	  //ϵͳʱ�ӳ�ʼ��
	delay_init();			  //��ʱ��ʼ��
	LcdIoInit();
	LcdInit();
	DisInit();
	Dht11IoInit();
	
	LcdWriteCmd(0x80);
	for(i=0; i<14; i++)
	{
		LcdWriteData(one[i]);
	}
	LcdWriteCmd(0xc0);
	for(i=0; i<10; i++)
	{
		LcdWriteData(two[i]);
	}

	while(1)
	{
		ReceiveData();		//���ö�ȡ��ʪ�Ⱥ���

		display(0x80+11,humih/10+0X30);		//�����ݷֽ��λ ʮλ  
		display(0x80+12,humih%10+0X30);		//+0x30 ��ʾ�����ݱ��10������ʾ

		display(0xC0+11,temph/10+0X30);
		display(0xC0+12,temph%10+0X30);
		
		delay_ms(120);		   //ÿ120msִ��һ�ζ�д
	}
}

/*�˺���������ʾ���϶ȵĵ�λ����*/
void DisInit()
{
	display(0xc0+13,0xdf);
	display(0xc0+14,0x43);
}
