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

/*�ú�����ʾ����ʪ������ַ�*/
void DisInit(void);	 

int main(void)
{
	Stm32_Clock_Init(9);	  //ϵͳʱ�ӳ�ʼ��
	delay_init(72);			  //��ʱ��ʼ��
	LcdIoInit();
	LcdInit();
	DisInit();
	Dht11IoInit();

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

void DisInit()
{
	display(0x80+1,'H');
	display(0x80+2,'u');
	display(0x80+3,'m');
	display(0x80+4,'i');
	display(0x80+5,' ');
	display(0x80+6,'N');
	display(0x80+7,'o');
	display(0x80+8,'w');
	display(0x80+9,':');
	display(0x80+13,'%');

	display(0xc0+1,'T');
	display(0xc0+2,'e');
	display(0xc0+3,'m');
	display(0xc0+4,'p');
	display(0xc0+5,' ');
	display(0xc0+6,'N');
	display(0xc0+7,'o');
	display(0xc0+8,'w');
	display(0xc0+9,':');
	display(0xc0+13,0xdf);
	display(0xc0+14,0x43);
}
