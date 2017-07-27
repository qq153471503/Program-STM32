#include <reg52.h>

#define u8 unsigned char
#define u16 unsigned short int
#define u32 unsigned int

sbit single = P0^0;

u16 PWM = 500;

//��ʱ����
//��ڲ�����ms
//��λ��΢��
//89ϵ�е�Ƭ����85
//12ϵ�е�Ƭ����800
void delay_ms(u16 ms)
{
	u16 i,k;
	for(i=0; i<85; i++)
		for(k=0; k<ms; k++);
}

//��ʱ����ʼ������
//��ڲ�������
//��ʱ��0 ������ʽ1
void Timer_Init(void)
{
	TMOD = 0X01;  			//�趨������ʽ
	TL0 = 0;				//װ��ʼֵ
	TH0 = 0;				//װ��ʼֵ
	TF0 = 0;				//����жϱ�־
	ET0 = 1;				//����ʱ���ж�
	TR0 = 1;				//�򿪶�ʱ��
}
//����Ƕȿ��ƺ���
//��ڲ�����PWM 
//PWM �� 500~2500
void Servo_Control(u16 PWM)
{
	u16 value;
	value = 0XFFFF-PWM;
	TL0 = value;
	TH0 = value>>8;
	TR0 = 1;
}

void main(void)
{
	Timer_Init();
	EA = 1;
	while(1)
	{
		PWM = 500;
		delay_ms(1000);
		PWM = 1000;
		delay_ms(1000);
		PWM = 1500;
		delay_ms(1000);
		PWM = 2000;
		delay_ms(1000);
		PWM = 2500;
		delay_ms(1000);
		PWM = 2000;
		delay_ms(1000);
		PWM = 1500;
		delay_ms(1000);
		PWM = 1000;
		delay_ms(1000);
	}
}

//��ʱ��0 �жϷ�����
void TIM0_IRQhandler(void) interrupt 1 using 1
{
	static u8 i = 1;
	
	switch(i)
	{
		case 1:
			single = 1;
			Servo_Control(PWM);
			break;
		case 2:
			single = 0;
			Servo_Control(20000-PWM);
			i = 0;
			break;
	}
	i++;	
}
