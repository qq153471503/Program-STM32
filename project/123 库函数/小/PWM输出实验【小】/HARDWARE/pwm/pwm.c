#include"pwm.h"

void PwmInit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;		//����ͨ�������
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//�����˿�x��ʱ��
	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;     //����I/O�˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //�������/���ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //ִ����������

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     //����I/O�˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�������/���ģʽ  �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //ִ������

	//��ʱ��TIMx��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE); 
 //��ʱ��x ��������ж� �������ж� ʹ��TIMx

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

//ѡ��ʱ��ģʽ: TIM_OCMode_PWM1;  TIM_OCMode_PWM2;

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�TIM_OCPolarity_Low;//��
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCRx(ͨ��x)�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
}



	
