#include "stm32f10x.h"
#include "djqd.h"
#include "canp_hostcom.h"
#include "test.h"

u8 R_Speed =0;
u8 L_Speed =0;

u8 flag_djzt =0;  // ���״̬��־λ
u8 timer6_num =0; // ��ʱ������


/***************************************************************
** ���ܣ�     ��������˿ڳ�ʼ��
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void DJ_Init(void)
{
	RCC->APB2ENR|=1<<6;    //ʹ��PORTEʱ��
	GPIOE->CRH&=0XF00F0000; 
	GPIOE->CRH|=0X03303333;   //PE8/PE9/PE10/PE11/PE13/PE14�������  	 
  GPIOE->ODR|=0X0000;

	LEFT_NSLEEP = 1;
	REFT_NSLEEP = 1;
	
	REFT1_PWM =1;
	REFT2_PWM =1;
	
	LEFT1_PWM =0;
	LEFT2_PWM =0;
	
	
}

/***************************************************************
** ���ܣ�     ͣ��
** ������	  ��
** ����ֵ��    ��	  
****************************************************************/
void STOP(void)
{
		u16 st;

	TIM_Cmd(TIM2, DISABLE);
	
//	for(st=0;st<10;st++)
//	{
//	  TIM1->CCR1=st*10;
//    TIM1->CCR2=st*10;
//	  TIM1->CCR3=st*10;
//    TIM1->CCR4=st*10;
//	}
	  TIM1->CCR1=100;
    TIM1->CCR2=100;
	  TIM1->CCR3=100;
    TIM1->CCR4=100;	
	
//	  TIM1->BDTR &=~(1<<15);  //����OC��OCN���
	
	  LEFT_NSLEEP = 1;
	  REFT_NSLEEP = 1;
	
	  Send_UpMotor(0 , 0);  // �ϴ�����ٶ�
	
}

/***************************************************************
** ���ܣ�     PWM��ʼ��
** ������	  frequency���Զ���װֵ
**            psc��ʱ��Ԥ��Ƶ��
** ����ֵ��    ��	  
****************************************************************/
void PWM_Init(void)
{	
	
	#if 1

//	u32 OCLK=72000000;		 					 
    //�˲������ֶ��޸�IO������
	RCC->APB2ENR|=1<<11; 	//TIM1ʱ��ʹ��  
  RCC->APB2ENR|=1<<6;    //ʹ��PORTEʱ��  
	RCC->APB2ENR|=1<<0;       //��������ʱ�� 
	
	GPIOE->CRH&=0XF00F0F0F;	//PE9/11/13/14���
	GPIOE->CRH|=0X0BB0B0B0;	//���ù������ 	  	 
	
   
	AFIO->MAPR&=0XFFFFFF3F; //���MAPR��[7:6]
	AFIO->MAPR|=1<<7;      //��ȫ��ӳ��,TIM1_CH1->PE9  L1   TIM1_CH2->PE11 L2 
	AFIO->MAPR|=1<<6;      //��ȫ��ӳ��,TIM1_CH3->PE13 R1   TIM1_CH4->PE14 R2 

//	TIM1->ARR=OCLK/frequency;//�趨�������Զ���װֵ 
//	TIM1->PSC=psc;			//Ԥ��Ƶ������Ƶ

	TIM1->ARR=100;        //�趨�������Զ���װֵ  ռ�ձȱ���0~100%
	TIM1->PSC=9;	      //10��Ƶ //72KHZ PWMƵ�����

	TIM1->CCMR1|=6<<4;  	//CH1 PWM1ģʽ		 
	TIM1->CCMR1|=1<<3; 	  //CH1 Ԥװ��ʹ��	
	
	TIM1->CCMR1|=6<<12;  	//CH2 PWM1ģʽ		 
	TIM1->CCMR1|=1<<11; 	//CH2 Ԥװ��ʹ��	
	
	TIM1->CCMR2|=6<<4;  	//CH3 PWM1ģʽ		 
	TIM1->CCMR2|=1<<3; 	  //CH3 Ԥװ��ʹ��	  

	TIM1->CCMR2|=6<<12;  	//CH4 PWM1ģʽ		 
	TIM1->CCMR2|=1<<11; 	//CH4 Ԥװ��ʹ��
	
	
	TIM1->CR1|=1<<7;     //ARPEʹ���Զ���װ��Ԥװ������λ
	TIM1->CR1|=1<<4; 	   //���¼���ģʽ

	TIM1->CCER|=1<<0;   //OC1 ���ʹ��   �ߵ�ƽ��Ч
	TIM1->CCER|=1<<4;   //OC2 ���ʹ��
	TIM1->CCER|=1<<8;   //OC3 ���ʹ��
	TIM1->CCER|=1<<12;  //OC4 ���ʹ��
	


	TIM1->EGR |= 1<<0; //��ʼ�����еļĴ���
	TIM1->CR1|=1<<0;    //ʹ�ܶ�ʱ��1		

	TIM1->CCR1= 50;
	TIM1->CCR2= 00;  
//	TIM1->BDTR |=1<<15;  //����OC��OCN���

#endif
	
}


/***************************************************************
** ���ܣ�     ������ƺ���
** ������	  L_Spend����������ٶ�
**            R_Spend����������ٶ�
** ����ֵ��    ��	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
	//	STOP();
	TIM_Cmd(TIM2, ENABLE);
	Send_UpMotor(L_Spend ,R_Spend);

  if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//�����ٶȲ���
     LEFT_NSLEEP =1;  TIM1->CCR2= L_Spend; TIM1->CCR1= 0;
	}
	else 
	{
    if(L_Spend<-100)L_Spend= -100;if(L_Spend>-5)L_Spend= -5;     //�����ٶȲ���
      LEFT_NSLEEP =1;  TIM1->CCR1= -L_Spend; TIM1->CCR2= 0; 
  }
	
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//�����ٶȲ���
     REFT_NSLEEP =1;  TIM1->CCR3= R_Spend; TIM1->CCR4= 0;
	}
	else
	{	
		if(R_Spend<-100)R_Spend= -100;if(R_Spend>-5)R_Spend= -5;		//�����ٶȲ���		
     REFT_NSLEEP =1;  TIM1->CCR4= -R_Spend; TIM1->CCR3= 0;
	}
	
	TIM1->BDTR |=1<<15;  //����OC��OCN���
		
}

void Go_Test( u8 sp ,u8 len)  // ��50���ٶ�ǰ������Ϊ50
{
    MP=0;G_Flag=1;	Stop_Flag=0;tempMP=0;			 					
		tempMP= len;  
		Car_Spend = sp;
	  Control(Car_Spend,Car_Spend);
}

void Back_Test(u8 sp ,u8 len )  // ��50���ٶȺ��˾���Ϊ50
{

	MP=0;B_Flag=1;	Stop_Flag=0;tempMP=0;
	tempMP= len;  
	Car_Spend = sp;
	Control(-Car_Spend,-Car_Spend);

}

void Left_Test( u8 sp)  // ��80���ٶ���ת��
{

	MP=0;L_Flag=1;	Stop_Flag=0;
	Car_Spend = sp;
	Host_Open_UpTrack( Up_Track_Speed );
	Control(-Car_Spend,Car_Spend);

}

void Right_Test(u8 sp)  // ��80���ٶ���ת��
{
   MP=0;R_Flag=1;	Stop_Flag=0;
	 Car_Spend = sp;
	 Host_Open_UpTrack( Up_Track_Speed );
	 Control(Car_Spend,-Car_Spend);

}

void Track_Test( u8 sp)  // ��50���ٶ�ѭ��
{

		Car_Spend = sp;
		Track_Flag=1;MP=0; 					  
		Host_Open_UpTrack( Up_Track_Speed );  // ����Ѱ�������ϴ�
		delay_ms( 100); // �ȴ�һ��Ѱ�������ϴ�
		TIM_Cmd(TIM2, ENABLE);

}

void Stop_Test(void)
{
		Track_Flag=0;MP=0;
		Stop_Flag=0;G_Flag=0;B_Flag=0;L_Flag=0;R_Flag=0; 
	  STOP();


}




