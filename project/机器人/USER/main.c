/*####################################################*/
		#include "sys.h"
		#include "delay.h"
		#include "usart.h"
		#include "servo.h"
		#include "led.h"
		#include "key.h"
		#include "system.h"
/*####################################################*/

//�����ʼ����λ����
void Servo_Homing(void);
//��������ǰ��ת����
void qian_fan(void);

static unsigned int t=0 ;
static unsigned int k=0;
	
int main(void)
{
//	RCC_Config();				//ʹ��HSI ϵͳ��ʼ��
	SystemInit();				//ʹ��HSE ϵͳ��ʼ��
	delay_init();				//��ʱ������ʼ��
	LED_Init();					//LED�Ƴ�ʼ��
	KEY_Init();					//������ʼ��
	Servo_Left_PWM(2000,719);	//��ʱ��2��ʼ��	1k�ļ���Ƶ�� ������2000��20ms   78/(719+1) = 1k
	Servo_Right_PWM(2000,719);	//��ʱ��3��ʼ��	1k�ļ���Ƶ�� ������2000��20ms   78/(719+1) = 1k
	Servo_Homing();				//���ȫ����λ���� ������������̬
	
	while(1)
	{
		qian_fan();
	}
	
}

/**
  * @brief  ���ж����ʼ�� ������������̬
  * @param  void
  * @retval None
  * @instructions  ��̬�����������������ļ����޷�����
  */
static __inline void Servo_Homing(void)
{	
	TIM_SetCompare1(TIM2,160);		//����
	TIM_SetCompare2(TIM2,150);		//����
	TIM_SetCompare3(TIM2,155);		//���
	
	TIM_SetCompare1(TIM3,165);		//��yao
	TIM_SetCompare2(TIM3,145);		//��zhong
	TIM_SetCompare3(TIM3,150);		//��jiao
	
	//�����������ƺ�,��ʱһ��
	delay_ms(1000);
}


/**
  * @brief  ʵ�ֻ�����ǰ����
  * @param  void
  * @retval None
  * @instructions  ��̬�����������������ļ����޷�����
  */
static __inline void qian_fan(void)
{
	for(t=150,k=145; t>=105,k<=190; t--,k++)
	{
		TIM_SetCompare2(TIM2,t);		//����
		TIM_SetCompare2(TIM3,k);		//��zhong
		delay_ms(15);
	}
	
	for(t=155,k=155; t>=115,k<=195; t--,k++)
	{
		TIM_SetCompare3(TIM2,t);		//���		
		TIM_SetCompare3(TIM3,k);		//��jiao
		delay_ms(20);
	}
	
	for(t=105,k=190; t>=55,k<=240; t--,k++)
	{
		TIM_SetCompare2(TIM2,t);		//����
		TIM_SetCompare2(TIM3,k);		//��zhong
		delay_ms(7);
	}
	
	for(t=160,k=165; t<=240,k>=80; t++,k--)
	{
		TIM_SetCompare1(TIM2,t);		//����
		TIM_SetCompare1(TIM3,k);		//��yao
		delay_ms(7);
	}
	
	for(t=115,k=195; t<=155,k>=155; t++,k--)
	{
		TIM_SetCompare3(TIM2,t);		//���		
		TIM_SetCompare3(TIM3,k);		//��jiao
		delay_ms(7);
	}
	
/**
 * �ط���ʼ
 ********/
	
	for(t=240,k=80; t>=120,k<=200; t--,k++)
	{
		TIM_SetCompare1(TIM2,t);		//����
		TIM_SetCompare1(TIM3,k);		//��yao
		delay_ms(12);
	}
	
	
	for(t=55,k=240; t<=245,k>=50; t++,k--)
	{
		TIM_SetCompare2(TIM2,t);		//����
		TIM_SetCompare2(TIM3,k);		//��zhong
		delay_ms(7);
	}

	for(t=120,k=200; t>=85,k<=235; t--,k++)
	{
		TIM_SetCompare1(TIM2,t);		//����
		TIM_SetCompare1(TIM3,k);		//��yao
		delay_ms(10);
	}

/**
 * ����
 *******/	
	for(t=245,k=50; t>=145,k<=150; t--,k++)
	{
		TIM_SetCompare2(TIM2,t);		//����
		TIM_SetCompare2(TIM3,k);		//��zhong	
		delay_ms(10);
	}
	
	for(t=90,k=235; t<=160,k>=165; t++,k--)
	{
		TIM_SetCompare1(TIM2,t);		//����
		TIM_SetCompare1(TIM3,k);		//��yao
		delay_ms(14);
	}
}



