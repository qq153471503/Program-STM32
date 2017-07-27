/************************************************************
**  Ƕ��ʽС�����İ������Ƴ�����
**	www.r8c.com
**  �汾�ţ�v2.0.3
**  ʱ��  ��2015.05.30
**  ����  ��BKRC
*************************************************************/
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "init.h"
#include "led.h"		 	 
#include "test.h"
#include "djqd.h"
#include "key.h"
#include "xj.h"
#include "csb.h"
#include "hw.h"
#include "74hc595.h"
#include "bh1750.h"

#define  NUM  10 	 // ����������ݳ���

void IO_Init(void); //IO��ʼ��	

//u8 mp;	
u8 G_Tab[6];	   //������ⷢ������
u8 S_Tab[NUM]; 	   //������������������
u8 C_Tab[NUM]; 	   //�����˶���־�ﷵ����������

u8 Stop_Flag=0;    //״̬��־λ
u8 Track_Flag=0;   //ѭ����־λ
u8 G_Flag=0;	   //ǰ����־λ
u8 B_Flag=0;	   //���˱�־λ
u8 L_Flag=0;	   //��ת��־λ
u8 R_Flag=0;	   //��ת��־λ
u8 SD_Flag=1;      //�˶���־�����ݷ��������־λ

u16 CodedDisk=0;   //����ֵͳ��
u16 tempMP=0;	   //��������ֵ
u16 MP;			   //��������ֵ
int Car_Spend = 70;//С���ٶ�Ĭ��ֵ
int count = 0;	   //����
int LSpeed;		   //ѭ�������ٶ�
int RSpeed;		   //ѭ�������ٶ�
u8 Line_Flag=0;

unsigned Light=0; //���ն�

static u8 H_S[4]={0x80,0x7F,0x05,~(0x05)};	 //��Ƭ�Ϸ�
static u8 H_X[4]={0x80,0x7F,0x1B,~(0x1B)};	 //��Ƭ�·�

static u8 H_1[4]={0x00,0xFF,0x0C,~(0x0C)};	 //��Դ��λ��1
static u8 H_2[4]={0x00,0xFF,0x18,~(0x18)};	 //��Դ��λ��2
static u8 H_3[4]={0x00,0xFF,0x5E,~(0x5E)};	 //��Դ��λ��3

static u8 H_SD[4]={0x00,0xFF,0x45,~(0x45)};	  //�������ϵͳ��

u8 HW_K[6]={0x03,0x05,0x14,0x45,0xDE,0x92};  //��������
u8 HW_G[6]={0x67,0x34,0x78,0xA2,0xFD,0x27};	//�������ر�

static u8 CP_G1[6]={0xFF,0x12,0x01,0x00,0x00,0x00};
static u8 CP_G2[6]={0xFF,0x13,0x01,0x00,0x00,0x00};

// ������
int main(void)
{
	
	u8 i;
	Stm32_Clock_Init(9);        // ϵͳʱ������
	delay_init(72);	            // ��ʱ��ʼ�� 
	uart1_init(72,115200);	    // ���ڳ�ʼ��Ϊ115200
	uart2_init(72,115200);	    // ���ڳ�ʼ��Ϊ115200
	IO_Init();                  //IO��ʼ��

	STOP();
	S_Tab[0]=0x55;
	S_Tab[1]=0xaa;

	C_Tab[0]=0x55;
	C_Tab[1]=0x02;
						
	while(1)
	{	 	   
		Track();
	}
}

/***************************************************************
** ���ܣ�     ��ʼ�����İ���ʹ�ö˿�
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void IO_Init(void)
{
	YJ_INIT();			//��ʼ��Ӳ��

	GPIOB->CRH&=0XFFFFFFF0;	  
	GPIOB->CRH|=0X00000008;//PB8 ���ó�����   			   
	GPIOB->ODR|=1<<8;	   //PB8����

	GPIOC->CRH&=0X000FFFFF; 
	GPIOC->CRH|=0X33300000;   //PC13/PC14/PC15�������  	 
    GPIOC->ODR|=0XE000;       //PC13/PC14/PC15�����
		   	 
	GPIOD->CRH&=0XFFF0FFFF; 
	GPIOD->CRH|=0X00030000;   //PD12�������  	 
    GPIOD->ODR|=0X1000;       //PD12�����											  
	
	LED_L=1;				  
	LED_R=1;
	BEEP=1;
	beep=1;
}
