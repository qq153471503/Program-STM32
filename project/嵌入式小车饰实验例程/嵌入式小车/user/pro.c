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
			len=USART_RX_STA&0x3f;					//�õ����ݳ���
			for(t=0;t<len;t++)
			{

				flag = USART1->DR = USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);				//�ȴ����ͽ���

					switch(flag)
					{
						case 'G':	Motor_Go();			break ;   //ǰ��				
						case 'B': 	Motor_Back();		break ;   //����
						case 'L':	Motor_Left();		break ;	  //��ת 
						case 'R': 	Motor_Right();		break ;   //��ת 
						case 'S':	Motor_Stop();		break ;	  //ͣ��
		
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
				led1 = !led1;				//��˸LED,��ʾϵͳ��������.
				led2 = !led2;
				led3 = !led3;
			}
			delayms(10);   
		}
	}
}




