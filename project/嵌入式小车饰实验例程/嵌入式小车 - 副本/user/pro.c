#include<stm32f10x_lib.h>
#include"led.h"
#include"usart.h"
#include"delay.h"
#include"timer.h"

 u16 PwmValue = 500;	   //���ٶȳ�ֵ

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
			len=USART_RX_STA&0x3f;					//�õ����ݳ���
			for(i=0; i<len; i++)
			{

				flag = USART1->DR = USART_RX_BUF[i];
				while((USART1->SR&0X40)==0);				//�ȴ����ͽ���

					switch(flag)
					{
						case 'G':	Motor_Go();			break ;   //ǰ��				
						case 'B': 	Motor_Back();		break ;   //����
						case 'L':	Motor_Left();		break ;	  //��ת 
						case 'R': 	Motor_Right();		break ;   //��ת 
						case 'S':	Motor_Stop();		break ;	  //ͣ��

						case 'a': PwmValue = 900 ; break ;					//һ���ٶ�
						case 'b': PwmValue = 700; break ;					//�����ٶ�
						case 'c': PwmValue = 500 ; break ;					//�����ٶ�
						case 'd': PwmValue = 300 ; break ;					//�ļ��ٶ�
						case 'e': PwmValue = 100 ; break ;					//�弶�ٶ�
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
				led1 = !led1;				//��˸LED,��ʾϵͳ��������.
				led2 = !led2;
				led3 = !led3;
				beep = !beep;
			}
			delayms(10);   
		}

		LeftPwm = RightPwm = PwmValue;

	}
}







