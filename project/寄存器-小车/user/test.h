#ifndef __TEST_H
#define __TEST_H	 
#include "sys.h"


#define LED_L PCout(15) //PC15 �������ƶ���
#define LED_R PCout(14) //PC14 ������ҵƶ���
#define BEEP PCout(13)  //PC13 ��������������
#define beep PDout(12)  //PD12 ���İ����������
#define PSS PBin(8)     //PB8  ��������״̬����

extern u8 Stop_Flag; 
extern u8 Track_Flag;
extern u8 G_Flag;
extern u8 B_Flag;
extern u8 L_Flag;
extern u8 R_Flag;
extern u16 NowMP;
extern u16 CodedDisk;
extern u16 tempMP;
extern u16 MP;
extern int Car_Spend;		 //ѭ���ٶ�
extern int count;
extern int corSpeed;		//ѭ�������ٶ�
extern int LSpeed;		   //ѭ�������ٶ�
extern int RSpeed;		   //ѭ�������ٶ�
extern u8 Line_Flag;


extern u8 HW_K[6];  //��������
extern u8 HW_G[6];	//�������ر�
	 				    
#endif


