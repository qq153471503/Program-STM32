/************************************************************
**  ����С�����İ���Ƴ���
**	www.r8c.com
**  �汾�ţ�V1.1
**  ʱ��  ��2017.03.08
**  ����  ��BKRC
**  �޸ļ�¼�� ����CAN�ӿ�
**  �Ƽ�ʹ�� ���� �� Ѱ���ٶ� 50   ת���ٶ�  80 
*************************************************************/
#include "stm32f10x.h"  //����
#include "sys.h"					
#include "usart.h"		
#include "delay_drv.h"
#include "init.h"
#include "led.h"		 	 
#include "exit.h"
#include "test.h"
#include "djqd.h"
#include "key.h"
#include "xj.h"
#include "csb.h"
#include "hw.h"
#include "74hc595.h"
#include "bh1750.h"
#include "uart_my.h"
#include "canp_hostcom.h"
#include "can_drv.h"
#include "fifo_drv.h"
#include "data_channel.h"
#include "power_check.h"
#include "syn7318.h"
#include "iic.h"
#include "hmc5883l.h"
#include "bz.h"
#include "zx.h"
#include "rc522.h"

#define  NUM  10 	 // ����������ݳ���
#define  ZCKZ_ADDR    0xAA  // ���������־���ַ���
#define  YSBZW_ADDR   0x02  // ���������־���ַ���
#define  DZ_ADDR      0x03  // �����բ��־���ַ���
#define  LEDXS_ADDR   0x04  // ����LED��ʾ��־���ַ���
#define  JXB_ADDR     0x05  // �����е�۱�־���ַ���


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
u8 Rx_Flag =0;
unsigned int R;

u16 CodedDisk=0;   //����ֵͳ��
u16 tempMP=0;	   //��������ֵ
u16 MP;			   //��������ֵ
int Car_Spend = 50;//С���ٶ�Ĭ��ֵ
int count = 0;	   //����
int LSpeed;		   //ѭ�������ٶ�
int RSpeed;		   //ѭ�������ٶ�
u8 Line_Flag=0;	   // 
u8 send_Flag=0;	   // ���ͱ�־λ
u8 ka=0,biao=1,ab,cd,xy;
u8 guang=0,ku=1;
u8 Kuai=1;
u8 anjian=1;
u8 tu1,tu2,tu3,mm=0;
u8 weizhi,fangxiang;
// ������

RCC_ClocksTypeDef RCC_Clocks;
u8 Card1[21]= {0x55,0xAA,0x09,0x09,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0xBB};
u8 Card2[16]=	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
u8 TabGQ[5];
u8 TabCSB[4];
static u8 GUANGDANG1[26]={"��������ϵͳԤ�赵λΪһ��"};	
void SysTick_Handler(void)
{
	global_times++;
	if(delay_ms_const)
		delay_ms_const--;
}


extern void pwm_test(void);

int main(void)
{
	u8 ut=0;
	u8 i;
	unsigned int a;
	global_times = 0;
	SystemInit();
  Delay_us_Init();
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	RC522_Uart_init(9600);
	InitRc522();
	RC522_Uart_init(115200);
	


	NVIC_Configuration(); 	 //����NVIC�жϷ���0:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
//	Xj_Track_Test();
	
	uart1_init(115200);	    // ���ڳ�ʼ��Ϊ115200

	uart2_init(115200, 0);	    // ���ڳ�ʼ��Ϊ115200 // Ĭ�Ͽ��ж�
	
	IO_Init();                  //IO��ʼ��

	S_Tab[0]=0x55;
	S_Tab[1]=0xaa;

	C_Tab[0]=0x55;
	C_Tab[1]=0x02;
	
	CanP_Init();
	
	Power_Check();  //�������  �ϵ��ȼ��һ�ε���
	Send_Electric( Electric_Buf[0],Electric_Buf[1]);
	

	STOP();
	Host_Close_UpTrack();
	
	Send_Debug_Info("A1B2C3\n",8); // �ϴ�������Ϣ
 	
	while(1)
	{	 	   
		
 	  ut ++;
		LED0=!LED0;	//����״̬

//		CanP_Host_Main();      // �ŵ���ʱ����ȥ��������
//		CanP_CanTx_Check();
//		
//		 SYN7318_Test();
	
		if( !(ut %100))
		{		
			Power_Check();  //�������  ����ʵʱ����

      Send_Electric( Electric_Buf[0],Electric_Buf[1]);
			
						
		}
		  S_Tab[2]=Stop_Flag;   	//С������״̬

		  if(PSS==1) S_Tab[3]=1;	//����״ֵ̬����
		     else S_Tab[3]=0;

		   tran();

		   S_Tab[4]=dis%256;        //����������
		   S_Tab[5]=dis/256;
		   	    
			  
		   S_Tab[8]=CodedDisk%256;  //����
		   S_Tab[9]=CodedDisk/256;
		

		 if(!KEY0)			  //����K1����
		 {
		   delay_ms(30);
		 if(!KEY0)
		 {
				Rc522(25,1);	
				Rc522(25,1);	
				Rc522(25,1);	
				WRITE_RFID[12]=0x1A;
				WRITE_RFID[13]=0xE5;
				WRITE_RFID[14]=0x1A;
				WRITE_RFID[15]=0xE5;
				Rc522(26,1);	
				Rc522(26,1);	
				Rc522(26,1);	
			    
		 }
		}
	if(!KEY1)			//����K2����
	{
		delay_ms(30);
		if(!KEY1)
		{	
			Rc522(25,0);
			Rc522(26,0);
		}
	}
	   if(!KEY2)			  //����K3����
		{
		   delay_ms(30);
		if(!KEY2)
		{
			
		}
		}
		if(!KEY3)			  //����K4����
		{
		  delay_ms(30);
	    if(!KEY3)
			{
				Rc522(25,0);	
			}
		}

	
		if(Wifi_Rx_flag ==1)  // wifi ���ձ��
		{
			  Wifi_Rx_flag =0;

			  if(Wifi_Rx_Buf[0]==0xFD)  // ���յ��������������ݣ�ֱ��ת��
			  {
					  send_data_zigbee( Wifi_Rx_Buf , (Wifi_Rx_num +1)); 
			  }
			  else if(Wifi_Rx_Buf[0]==0x55)  // ���յ�55��ͷ����
			  {
			  	 
			  	  Normal_data();  //�������ݴ���
			  }
			  else {
			  	   Abnormal_data();      // �쳣���ݴ���
			  }
		
		}

		if(Rx_Flag ==1)	//���յ�����ָ��
		{

		STOP();
		delay_us(5);
		Rx_Flag=0;	

		if(Wifi_Rx_Buf[1]==ZCKZ_ADDR) 	   //��������
		{
//			beep=1;
//			delay_ms(100);
//			beep=0;
				if(Wifi_Rx_Buf[2]==0x01)
				{							
					line=5;
		
					while(1)
					{
						gongzuo();
					}
					
				}
				
	    	Rx_Flag =0;
		}

		else  send_data_zigbee( Wifi_Rx_Buf ,8);			  // ����������ת��������zigbee�ڵ�

		} 
		
		if(Zigbee_Rx_flag ==1)	 //zigbee������Ϣ
		{
		  STOP()  ; 
	      delay_us(5);

			if( (Zigb_Rx_Buf[1]==0x03) || (Zigb_Rx_Buf[1]==0x0c)) // ��բ �� ETC
			{
			   if(Zigb_Rx_Buf[2]==0x01)
			   {
					if(Zigb_Rx_Buf[3]==0x01)
					{
						Stop_Flag=Zigb_Rx_Buf[4] ;	  // 05 ������բ���  06 ETC
						Zigbee_Rx_flag =0;
					}
					else Zigbee_Rx_flag =0;			   

			   }
			    else Zigbee_Rx_flag =0;
			
			}	

			else if(Zigb_Rx_Buf[1]==0x21)	 //��һ������
			  {

					C_Tab[2]=Zigb_Rx_Buf[2];
					C_Tab[3]=Zigb_Rx_Buf[3];
					C_Tab[4]=Zigb_Rx_Buf[4];
					C_Tab[5]=Zigb_Rx_Buf[5];
					//flag3 = 1;

				}
		     else if(Zigb_Rx_Buf[1]==0x22)	 //�ڶ�������
				{

					C_Tab[6]=Zigb_Rx_Buf[2];
					C_Tab[7]=Zigb_Rx_Buf[3];
					C_Tab[8]=Zigb_Rx_Buf[4];
					C_Tab[9]=Zigb_Rx_Buf[5];

					flag3 = 1;
				}
			  
			else if(Zigb_Rx_Buf[1]==0x06)
			   {
				if(Zigb_Rx_Buf[2]==0x01)
					{
						Stop_Flag=Zigb_Rx_Buf[3] ;	  // ����оƬ״̬����
						Zigbee_Rx_flag =0;
					}
					else Zigbee_Rx_flag =0;
		     	}

			   else Zigbee_Rx_flag =0;


			 Zigbee_Rx_flag =0;		
		}

		if(send_Flag==1)
		{
// 		   for(i=0;i<Zigb_Rx_Buf[2];i++)		//��е������ͨ������1�������
// 			{
// 				 U1SendChar(Zigb_Rx_Buf[i]);
// 			}
			 send_data_wifi(Zigb_Rx_Buf ,Zigb_Rx_Buf[2]);
			
		   send_Flag=0;

	    }

		else if((flag3==1)&&(SD_Flag==0))
		  {
// 		  for(i=0;i<10;i++)	 // ������ƽ����Է��������־������
// 		    {
// 	          U1SendChar(C_Tab[i]);
// 		    }
				send_data_wifi(C_Tab , 10);
				
			flag3 = 0;
	  	   }
		else if((SD_Flag))
		   {
		 
 //		  		 send_data_wifi(S_Tab , 10);
				 
		    }
	
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
	
	beep=0;
		
}
