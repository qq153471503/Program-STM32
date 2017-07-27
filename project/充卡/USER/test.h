#ifndef __TEST_H
#define __TEST_H	 
#include "sys.h"


#define LED_L PCout(15) //PC15 �������ƶ���
#define LED_R PCout(14) //PC14 ������ҵƶ���
#define BEEP PCout(13)  //PC13 ��������������
#define beep PDout(12)  //PD12 ���İ����������
#define PSS PBin(8)     //PB8  ��������״̬����

#define  Up_Track_Speed  1  //����Ѱ�����ݷ���ʱ���� ��λms 

static u8 H_S[4]={0x80,0x7F,0x05,~(0x05)};	 //��Ƭ�Ϸ�
static u8 H_X[4]={0x80,0x7F,0x1B,~(0x1B)};	 //��Ƭ�·�

static u8 H_1[4]={0x00,0xFF,0x0C,~(0x0C)};	 //��Դ��λ��1
static u8 H_2[4]={0x00,0xFF,0x18,~(0x18)};	 //��Դ��λ��2
static u8 H_3[4]={0x00,0xFF,0x5E,~(0x5E)};	 //��Դ��λ��3

static u8 H_SD[4]={0x00,0xFF,0x45,~(0x45)};	  //�������ϵͳ��

static u8 HW_K[6]={0x03,0x05,0x14,0x45,0xDE,0x92};  //��������
static u8 HW_G[6]={0x67,0x34,0x78,0xA2,0xFD,0x27};	//�������ر�

static u8 CP_G1[6]={0xFF,0x12,0x01,0x00,0x00,0x00};
static u8 CP_G2[6]={0xFF,0x13,0x01,0x00,0x00,0x00};

static u8 DZ_K[8]={0x55,0x03,0x01,0x01,0x00,0x00,0x02,0xBB};  //��բ����
static u8 DZ_G[8]={0x55,0x03,0x01,0x02,0x00,0x00,0x03,0xBB};  //��բ�ر�	

static u8 SMG_SHOW[8]={0x55,0x04,0x02,0xA1,0xB2,0xC3,0x18,0xBB};  //�������ʾ
static u8 SMG_JSK[8]={0x55,0x04,0x03,0x01,0x00,0x00,0x04,0xBB};  //����ܿ�ʼ��ʱ
static u8 SMG_JSG[8]={0x55,0x04,0x03,0x00,0x00,0x00,0x03,0xBB};  //����ܹرռ�ʱ
static u8 SMG_JL[8]={0x55,0x04,0x04,0x00,0x02,0x00,0x06,0xBB};  //�������ʾ����

static u8 CP_SHOW1[6]={0xFF,0x20,'E','4','6','7'};
static u8 CP_SHOW2[6]={0xFF,0x10,'I','7','B','2'};
extern u8 Card1[21];
extern u8 Card2[16];
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
extern u8 ka;
extern int Car_Spend;		 //ѭ���ٶ�
extern int count;
extern int corSpeed;		//ѭ�������ٶ�
extern int LSpeed;		   //ѭ�������ٶ�
extern int RSpeed;		   //ѭ�������ٶ�
extern u8 Line_Flag;
extern u8 send_Flag;	   // ���ͱ�־λ
extern u8 Rx_Flag;
extern u8 biao;
extern u8 ab,cd,xy;
extern u8 guang,ku,Kuai,anjian;
extern u8 tu1,tu2,tu3,mm;
extern u8 weizhi,fangxiang;
	 				    
#endif


