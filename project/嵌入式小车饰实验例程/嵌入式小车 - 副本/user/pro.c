#include<stm32f10x_lib.h>
#include"led.h"
#include"usart.h"
#include"delay.h"
#include"timer.h"

 u16 PwmValue = 500;	   //给速度初值

int main(void)
{
	u8 flag ;
	u8 i;
	u8 len;	
	u16 timescount = 0; 
	 
		Stm32ClockInit(9);
		DelayInit(72);
		LedInit();
		uart_init(72,9600);	 
		PwmInit(900,0);

	while(1)   
	{
		if(USART_RX_STA&0x80)
		{					   
			len=USART_RX_STA&0x3f;					//得到数据长度
			for(i=0; i<len; i++)
			{

				flag = USART1->DR = USART_RX_BUF[i];
				while((USART1->SR&0X40)==0);				//等待发送结束

					switch(flag)
					{
						case 'G':	Motor_Go();			break ;   //前进				
						case 'B': 	Motor_Back();		break ;   //后退
						case 'L':	Motor_Left();		break ;	  //左转 
						case 'R': 	Motor_Right();		break ;   //右转 
						case 'S':	Motor_Stop();		break ;	  //停车

						case 'a': PwmValue = 900 ; break ;					//一级速度
						case 'b': PwmValue = 700; break ;					//二级速度
						case 'c': PwmValue = 500 ; break ;					//三级速度
						case 'd': PwmValue = 300 ; break ;					//四级速度
						case 'e': PwmValue = 100 ; break ;					//五级速度
						case 'f': PwmValue = 0;	break ;
								
						default: break ;
					}
			}
			USART_RX_STA=0;
		}
		else
		{
			timescount++; 
			if(timescount%50==0)
			{
				led0 = !led0;
				led1 = !led1;				//闪烁LED,提示系统正在运行.
				led2 = !led2;
				led3 = !led3;
				beep = !beep;
			}
			delayms(10);   
		}

		LeftPwm = RightPwm = PwmValue;

	}
}







