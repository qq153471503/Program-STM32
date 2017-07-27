#include <reg52.h>

#define u8 unsigned char
#define u16 unsigned short int
#define u32 unsigned int

sbit single = P0^0;

u16 PWM = 500;

//延时函数
//入口参数：ms
//单位：微秒
//89系列单片机用85
//12系列单片机用800
void delay_ms(u16 ms)
{
	u16 i,k;
	for(i=0; i<85; i++)
		for(k=0; k<ms; k++);
}

//定时器初始化函数
//入口参数：无
//定时器0 工作方式1
void Timer_Init(void)
{
	TMOD = 0X01;  			//设定工作方式
	TL0 = 0;				//装初始值
	TH0 = 0;				//装初始值
	TF0 = 0;				//清楚中断标志
	ET0 = 1;				//开定时器中断
	TR0 = 1;				//打开定时器
}
//舵机角度控制函数
//入口参数：PWM 
//PWM ： 500~2500
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

//定时器0 中断服务函数
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
