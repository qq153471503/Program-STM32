#include"led.h"
#include"usart.h"
#include"delay.h"
#include"motor.h"

int main(void)
{
	u8 flag ;
	u8 t;
	u8 len;	
	u16 times=0; 
	 
		Stm32ClockInit(9);
		DelayInit(72);
		LedInit();
		uart_init(72,9600);	 
		MotorInit();

	while(1)   
	{
		if(USART_RX_STA&0x80)
		{					   
			len=USART_RX_STA&0x3f;					//得到数据长度
			for(t=0;t<len;t++)
			{

				flag = USART1->DR = USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);				//等待发送结束

					switch(flag)
					{
						case 'G':	Motor_Go();			break ;   //前进				
						case 'B': 	Motor_Back();		break ;   //后退
						case 'L':	Motor_Left();		break ;	  //左转 
						case 'R': 	Motor_Right();		break ;   //右转 
						case 'S':	Motor_Stop();		break ;	  //停车
		
						default: break ;
					}

			}
			USART_RX_STA=0;
		}
		else
		{
			times++; 
			if(times%60==0)
			{
				led0 = !led0;
				led1 = !led1;				//闪烁LED,提示系统正在运行.
				led2 = !led2;
				led3 = !led3;
			}
			delayms(10);   
		}
	}
}




