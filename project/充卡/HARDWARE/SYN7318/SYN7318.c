
#include "syn7318.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "djqd.h"
#include "test.h"
#include "hw.h"
#include "syn7318.h"
#include "canp_hostcom.h"
#include "data_channel.h"
u8 a=0,RE_flag;
unsigned char Ysn7813_flag=0;
unsigned char Ysn7813_flag_cs=0;

//	                         ֡ͷ  ���ݳ��� ���� ���� 04/�Զ��� 31=0X1F
unsigned char Wake_Up[] = {0xfd,0x00,0x02,0x51,0x1F};
unsigned char Stop_Wake_Up[] = {0xFD,0x00,0x01,0x52};
//	unsigned char Play_MP3[] = {0xFD,0x00,0x18,0x61,0x01,0x44,0x3A,0x5C,0x4D,0x70,0x33,0x5C,0x57,0x69,0x66,0x69,0x5F,0xCE,0xD2,0xD4,0xDA,0xD5,0xE2,0x2E,0x6D,0x70,0x33};

  unsigned char Play_MP3[] ={ 0xFD,0x00,0x1E,0x01,0x01,0xC6,0xF4,0xB6,0xAF,0xD3, 0xEF ,0xD2, 0xF4,
                            	0xBF, 0xD8, 0xD6 ,0xC6 ,0xBC ,0xDD ,0xCA ,0xBB ,0xA3 ,0xAC, 0xC7, 0xEB,
                             	0xB7, 0xA2, 0xB3, 0xF6 ,0xD6, 0xB8, 0xC1, 0xEE };
//	0xFD 0x00 0x1E 0x01 0x01 0xC6 0xF4 0xB6 0xAF 0xD3 0xEF 0xD2 0xF4 0xBF 0xD8 0xD6 0xC6 0xBC 0xDD 0xCA 0xBB 0xA3 0xAC 0xC7 0xEB 0xB7 0xA2 0xB3 0xF6 0xD6 0xB8 0xC1 0xEE 
//   0xFD 0x00 0x23 0x01 0x01 0xFD 0x00 0x1E 0x01 0x01 0xBB 0xB6 0xD3 0xAD 0xB9 0xDB 0xBF 0xB4 0xD3 0xEF 0xD2 0xF4 0xBA 0xCF 0xB3 0xC9 0xCF 0xB5 0xCD 0xB3 0xB5 0xC4 0xD1 0xDD 0xCA 0xBE 0xA3 0xA1
	//                             ֡ͷ  ���ݳ��� ʶ�� �ʵ�	
unsigned char Start_ASR_Buf[] = {0xFD,0x00,0x02,0x10,0x03};
unsigned char Stop_ASR_Buf[] = {0xFD,0x00,0x01,0x11};

uchar D_MP3_1[] = {"D:\\Mp3\\Wifi_�״ο���.mp3"};
uchar D_MP3_2[] = {"D:\Mp3\Prompt_������Ը�.mp3"};
uchar D_MP3_3[] = {"D:\Mp3\Answer_��Ǹû����.mp3"};
uchar D_MP3_4[] = {0xFD,0x00,0x21,0x15,0x03,0x04,0x02,0x01,0x44,0x3A,0x5C,0x4D,0x70,0x33,0x5C,0x50,0x72,0x6F,0x6D,0x70,0x74,0x5F,0xD6,0xF7,0xC8,0xCB,0xC7,0xEB,0xB7,0xD4,0xB8,0xC0,0x2E,0x6D,0x70,0x33};
uchar Stop_Th2O[] = {0xFD,0x00,0x01,0x16};
uchar TF_MP3_1[] = {"E:\\1-����\\һ�����㲻����.mp3"};

uchar *Dict0[] = {"������","������","�������","��С����","����ʾ��","�ر���ʾ��","�뿪��","��ص�","����һ��","����һ��","��С����","�������"};//00�ʵ�
uchar *Dict1[] = {"���","���޴���","������Ϸ","����֮��","���ؼҿ���","��ʱ�հ���","������Ϸ","��Խ����","��ʪ����","��Ĭ�ǽ�","���˶Դ�","������¹�"};//01�ʵ�
uchar *Dict2[] = {"һ�����㲻����","��ѧ��","��ָ��","������","�ɰ���̫��","��СѼ","���ֵܵĹ���","�ᳪ�������","ũ�����","�����̫��","С͵�͹���","������"};//02�ʵ�
uchar *Dict3[] = {"��һ����ò������õĴ���"};//03�ʵ� ��Ҫ�Լ�������¼����������еĴ���  
uchar ** DICT[10] = {Dict0,Dict1,Dict2,Dict3};

uchar Back[4] = {0};   //��������ش�������
uchar ASR[6] = {0};    //����ʶ�����ش�������
uchar S[4] = {0};      //����ģ�鵱ǰ����״̬�ش�������


/*********************************************************************
���� �� ������Delay1ms
������˵�������޲�
����    ������Delay1ms();��ʱ1ms��
*********************************************************************/
void Delay1ms()		 
{
			delay_ms(1);

}

/*********************************************************************
���� �� ������Delay500ms
������˵�������޲�
����    ������Delay500ms();��ʱ500ms��
*********************************************************************/
void Delay500ms()	 
{
     delay_ms(500);
}

#define USART_USE     0 // 1 USART1 0 USART2

/*********************************************************************
���� �� ������SYN7318_Init
������˵�������޲�
����    ������SYN7318_Init();��ʼ������������������Ϊ115200bps
*********************************************************************/
void SYN7318_Init(void)
{
#if  USART_USE
    uart1_init( 115200 );
#else 
	  uart2_init( 115200 ,1);
#endif
	
  RCC->APB2ENR |= 1<<5;	 //PD4
	GPIOD->CRL&=0XFFF0FFFF;//PD4 �������
	GPIOD->CRL|=0X00030000;	   
	GPIOD->ODR|=1<<4;     //PD4 �����
	
	
}

u8  SYN7318_Rst(void)  //����ģ�鸴λ
{
   SYN7318_RST_H;	
	 delay_ms(10);
	 
   SYN7318_RST_L;	
	 delay_ms(100);	

   SYN7318_RST_H;  
	
	while ( 0x55 == SYN7318_Get_char() );
	SYN7318_Get_String(Back ,3 );
	if(Back[2] ==0x4A )  return 1;
	else return 0;
	
}

void SYN7318_Open(void)
{

   Ysn7813_flag =1;
	 Ysn7813_flag_cs =1;

}

void SYN7318_Close(void)
{

  Ysn7813_flag = 0;
	Ysn7813_flag_cs = 0;
//	SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ

}



/*********************************************************************
���� �� ������SYN7318_Put_Char----����һ���ֽں���
������˵������txd---�����͵��ֽڣ�8λ��
����    ������SYN7318_Put_Char('d');  ���͡�d��
*********************************************************************/
void SYN7318_Put_Char(uchar txd)
{
#if  USART_USE	
	  U1SendChar( txd );
#else 
	  U2SendChar( txd );
#endif	
	
	
	
}

/*********************************************************************
���� �� ������SYN7318_Put_String----�����ַ�������
������˵������Pst������ַ�����������
              Length���ַ�������
����    ������uchar d[4] = {0x00,0x01,0x02,0x03};
              SYN7318_Put_String(d,4); ---��������d�е�Ԫ��
*********************************************************************/
void SYN7318_Put_String(uchar* Pst,uint Length)
{ 
   	uint i;
	for(i = 0; i < Length; i++)
	{
		  SYN7318_Put_Char( Pst[i]);
	}
}

/*********************************************************************
���� �� ������SYN7318_Get_char----����һ���ֽں���
������˵�������޲�
���� �� ֵ�������յ����ֽ�
����    ������uchar d;
              d = SYN7318_Get_char();
              SYN7318_Get_char(d); -----������յ����ֽ�
*********************************************************************/
#define WaitTime 10000
uchar SYN7318_Get_char( void) 
{
  	unsigned int return_data=0;	
#if  USART_USE	
	
	  while((!(USART1->SR & USART_FLAG_RXNE))||a>1000)
		{
			a++;
		}
	   return_data = (uint8_t)USART_ReceiveData(USART1);
 
#else 	
		while(!(USART2->SR & USART_FLAG_RXNE));
		return_data = (uint8_t)USART_ReceiveData(USART2);
#endif	
	
	return return_data;
	
}

/*********************************************************************
���� �� ������SYN7318_Get_String----�����ַ�������
������˵������Pst����Ž��յ����ַ�����������
              Length���ַ�������
����    ������uchar d[4] = {0};
              SYN7318_Get_String(d,4);
              SYN7318_Get_String(d,4);----������յ����ַ��� 
*********************************************************************/
void SYN7318_Get_String(unsigned char *Pst,unsigned char Length)
{
  unsigned char i;
  for(i=0;i<Length;i++)
  {
     Pst[i] = SYN7318_Get_char();
  }
	
}

/*********************************************************************
���� �� ������SYN_TTS----�����ϳɲ��ź���
������˵������Pst�����Ҫ�ϳɲ��ŵ��ı���������
����    ������uchar Data[] = {"��������"};
              SYN_TTS(Data); -----�ϳɲ��ű�������
*********************************************************************/
void SYN_TTS(uchar *Pst)
{
	uint Length;
	uchar Frame[5];   //���淢�����������
	
	
	Length = strlen(Pst); 
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x01;      //�����ϳɲ�������
	Frame[4] = 0x00;      //���ű����ʽΪ��GB2312��
	
  SYN7318_Put_String(Frame, 5);	
	SYN7318_Put_String(Pst, Length);	
	SYN7318_Get_String(Back,4);
	
	//���ճɹ�
	while(!(Back[3] == 0x41))
	{
		SYN7318_Get_String(Back,4);
	}	
	//���м��
	SYN7318_Get_String(Back,4);
	while(!(Back[3] == 0x4f))
	{
		SYN7318_Get_String(Back,4);
	}	
}

/*********************************************************************
���� �� ������SYN_MP3----MP3���ź���
������˵������Pst�����Ҫ���ŵ�MP3��������
����    ������uchar D_MP3_1[] = {"D:\\Mp3\\Wifi_�״ο���.mp3"};
              SYN_MP3(D_MP3_1); ----����MP3
*********************************************************************/
void SYN_MP3(uchar *Pst)
{
	uint Length;
	uchar Frame[5];   //���淢�����������
	
	Length = strlen(Pst); 
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x61;      //MP3��������
	Frame[4] = 0x04;      //���ű����ʽΪ��GB2312��
	
  SYN7318_Put_String(Frame, 5);	
	SYN7318_Put_String(Pst, Length);	
	SYN7318_Get_String(Back,0);
//	if(Back[3] == 0x41)
//	   LED2 = 0;
}

/*********************************************************************
���� �� ������Start_ASR----��ʼ����ʶ����
������˵������Dict���ʵ���
����    ������Start_ASR(0x00); ----ʶ��0x00�ʵ��еĴ���
*********************************************************************/
void Start_ASR(uchar Dict)
{

	uchar Frame[5];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x10;      //��ʼ����ʶ������
	Frame[4] = Dict;      //�ʵ���  �������޸���Ҫʶ��Ĵʵ���
	
	ASR[3]=0;
	
  SYN7318_Put_String(Frame, 5);	
	SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		 SYN7318_Get_String(Back,3);  //����ʶ������ش����
		if(Back[0] == 0xfc)
		{
			 SYN7318_Get_String(ASR,Back[2]);
		}
	}
}

/*********************************************************************
���� �� ������Stop_ASR----ֹͣ����ʶ����
������˵�������޲�
*********************************************************************/
void Stop_ASR()
{
	uchar Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x11;      //ֹͣ����ʶ������
	
  SYN7318_Put_String(Frame, 4);	
	SYN7318_Get_String(Back,4); 
//	if(Back[3] == 0x41)
//	{
//	   LED8 = 0; //D7��
//	}
}

/***************************************************************************
���� �� ������Dict_TTS----ʶ��ʵ�����������
������˵������Dict���ʵ���
����    ������Start_ASR(0x00); ----ʶ��0x00�ʵ��еĴ���
              Dict_TTS(0x00);------���ʶ��ɹ�,��ʶ�𵽵Ĵ����������� 
***************************************************************************/
void Dict_TTS(uchar Dict)
{
	switch(ASR[0])
	{
		case 0x01:
		case 0x02:
		{
			SYN_TTS(DICT[Dict][ASR[3]]);  //ASR[3]�д�Ŵ���ID
		}break;
		case 0x03:
		{
			SYN_TTS("�û�������ʱ");
		}break;
		case 0x04:
		{
			SYN_TTS("�û�������ʱ");
		}break;
		case 0x05:
		case 0x07:
		{
			SYN_TTS("ʶ���ʶ");
		}break;
		case 0x06:
		{
			SYN_TTS("ʶ���ڲ�����");
		}break;
	}
}

/*********************************************************************************************************************************************************
���� �� ������Start_WakeUp----��ʼ�������Ѻ���
������˵������Wake_ID��������ID��----0x00���Ʊ�  0x02��С��   0x04������  0x06�������ܼ�  0x07������ܼ�  0x08��С���ܼ�  0x09����ܼ�  0x1F���Զ���
����    ������Start_WakeUp(0x09);----������Ϊ��ܼ�,���ѳɹ�֮�󲥱��������⡱
**********************************************************************************************************************************************************/
void Start_WakeUp(uchar Wake_ID)
{
	uchar Frame[5];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x51;      //��ʼ������������
	Frame[4] = Wake_ID;   
	
  SYN7318_Put_String(Frame,5);	
	SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	//while(Back[3] == 0x41)
	{
		 SYN7318_Get_String(Back,4);  //������������ش����
		 if(Back[3] == 0x21)
		 //while(Back[3] == 0x21)
		 {
			 SYN_MP3(D_MP3_2);
		 }
	}
}

/*********************************************************************************************************************************************************
���� �� ������Three_One----����һ�����������������ѣ����Ѻ󲥷���ʾ���������������ʶ��
������˵������Dict���ʵ���
              Wake_ID��������ID��----0x00���Ʊ�  0x02��С��   0x04������  0x06�������ܼ�  0x07������ܼ�  0x08��С���ܼ�  0x09����ܼ�  0x1F���Զ���
              Sound����ʾ������  0x00������ʾ��  0x01���ı�����  0x02������MP3����
              Pst����ʾ��������
����    ������Three_One(0x00, 0x06, 0x02, D_MP3_2); ----������Ϊ�����ܼ�,���ѳɹ��󲥷š������⡱,֮�����ʶ��0x00�ʵ��еĴ��� 
              Dict_TTS(0x00); ----���Dict_TTS������ʶ������������
**********************************************************************************************************************************************************/
void Three_One(uchar Dict, uchar Wake_ID, uchar Sound, uchar *Pst)
{
	uchar Frame[100];   //���淢�����������
	uint Length,i;
	
	Frame[0] = 0xFD;      //֡ͷ
//	Frame[1] = 0x00;
//	Frame[2] = 0x07;    //���ڲ�ȷ���������ĳ��ȣ�����ѡ����ʱ����
	Frame[3] = 0x15;      //����һʶ������
	Frame[4] = Dict;      //�ʵ���
	Frame[5] = Wake_ID;   //������ID��
	Frame[6] = Sound;     //��ʾ������
	if(Sound == 0x00)     //����ʾ��
	{
		Frame[1] = 0x00;
		Frame[2] = 0x04;
	  SYN7318_Put_String(Frame, 7);	
	}
	else                 //����ʾ��
	{
		Frame[7] = 0x01;      //�ı�������MP3��ʾ�������ʽ��0x00��GB2312����  0x01��GBK����  0x02��BIG5����  0x03��UnicodeСͷ  0x04�Unicode��ͷ
		Length = strlen(Pst); //�ı�������MP3��ʾ���ĳ���
		Frame[1] = 0x00;
		Frame[2] = Length + 5;
		for(i = 0; i < Length; i++)
		{
			Frame[8 + i] = Pst[i];
		}
	  SYN7318_Put_String(Frame, Length + 8);	
	}
	 SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		SYN7318_Get_String(Back,3);  //����ʶ������ش����
		if(Back[0] == 0xfc)
		{
			 SYN7318_Get_String(ASR,Back[2]);
		}
	}
}

/***************************************************************************
���� �� ������Status_Query----ģ��״̬��ѯ����
������˵�������޲�
����    ������Status_Query(); 
***************************************************************************/
void Status_Query()
{
	uchar Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;    //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x21;   //״̬��ѯ����
	
  SYN7318_Put_String(Frame, 4);	
	SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 SYN7318_Get_String(S,4);  //ģ�鵱ǰ����״̬�Ļش����
	}
}
/*
<------   ���ճɹ�             0xFC 0x0 0x1 0x41 
<------   ʶ���ʶ             0xFC 0x0 0x1 0x7 

����һ

<------   ʶ��ɹ�,ƥ���=9,����ID=16,����ID=117    0xFC 0x0 0x6 0x1 0x9 0x0 0x10 0x0 0x75 
<------   0xFD,0x00,0x21,0x15,0x03,0x04,0x02,0x01,0x44,0x3A,0x5C,0x4D,0x70,0x33,0x5C,0x50,0x72,0x6F,0x6D,0x70,0x74,0x5F,0xD6,0xF7,0xC8,0xCB,0xC7,0xEB,0xB7,0xD4,0xB8,0xC0,0x2E,0x6D,0x70,0x33
          ֡ͷ   ����  ������ �ʵ� ���� ���� GBK  ����--->

����

���������ѡ�: len= 5   FD 00 02 51 04 
<------   ���ճɹ�     0xFC 0x0 0x1 0x41 

�������ϳɲ��š�: len= 22   FD 00 13 01 01 5B 6D 35 35 5D A1 B0 B0 D9 C1 E9 A1 B1 5B 6D 33 5D 
<------   ���ѳɹ�          0xFC 0x0 0x1 0x21 
<------   ���ճɹ�          0xFC 0x0 0x1 0x41 
<------   ģ�����          0xFC 0x0 0x1 0x4F 
*/

#define SYN_RST_MODE   1  // 0 ����λ   1 ������λ

void SYN7318_Test( void)  // ������������
{ 
	unsigned int i = 0;

	
//	SYN7318_Init();
	  

  
//	Ysn7813_flag_cs =1;
//	while( Ysn7813_flag_cs )
	if(Ysn7813_flag_cs )
	{

#if 	SYN_RST_MODE	
		while( !SYN7318_Rst());
#else 
		
#endif
	//	Ysn7813_flag_cs =0;
		
		SYN_TTS("����ʶ�����,�뷢������ʻ");
		LED2 = !LED2;
		Status_Query();  //��ѯģ�鵱ǰ�Ĺ���״̬
		delay_ms(1000);
		delay_ms(1000);
		Zigb_Rx_Buf[0]=0x55;
		Zigb_Rx_Buf[1]=0x06;
		Zigb_Rx_Buf[2]=0x10;
		Zigb_Rx_Buf[3]=0x01;
		Zigb_Rx_Buf[4]=0x00;
		Zigb_Rx_Buf[5]=0x00;
		Zigb_Rx_Buf[6]=0x11;
		Zigb_Rx_Buf[7]=0xBB;
		Send_ZigbeeData_To_Fifo(Zigb_Rx_Buf,8);
		if(S[3] == 0x4F)  //ģ����м���������
		{
			Delay1ms();
			
			SYN7318_Put_String(Wake_Up,5);//���ͻ���ָ��
			SYN7318_Get_String(Back,4);   //���շ�����Ϣ
		if(Back[3] == 0x41)         //���ճɹ�
		{ 
			SYN7318_Get_String(Back,3); //����ǰ��λ�ش�����
			if(Back[0] == 0xfc)       //֡ͷ�ж�
			{
				SYN7318_Get_String(ASR,Back[2]);//���ջش�����
				if(ASR[0] == 0x21)            //���ѳɹ�
				{
					SYN7318_Put_String(Play_MP3,33);//���š������⡱ 

					SYN7318_Get_String(Back,4);
					SYN7318_Get_String(Back,4);
					while(!(Back[3] == 0x4f));    //�ȴ�����
					{
						LED2 = ~LED2;
						Delay500ms();
					}
		//							Ysn7813_flag = 1;
		/*************************************************/
					//��ʼ����ʶ��
					while(Ysn7813_flag)
					{
						
						SYN7318_Put_String(Start_ASR_Buf,5);//������ʶ������
						SYN7318_Get_String(Back,4);     //���շ�����Ϣ
						Zigb_Rx_Buf[0]=0x55;
						Zigb_Rx_Buf[1]=0x06;
						Zigb_Rx_Buf[2]=0x10;
						Zigb_Rx_Buf[3]=0x00+tu1;
						Zigb_Rx_Buf[4]=0x00;
						Zigb_Rx_Buf[5]=0x00;
						Zigb_Rx_Buf[6]=(0x10+tu1)%0xFF;
						Zigb_Rx_Buf[7]=0xBB;
						Send_ZigbeeData_To_Fifo(Zigb_Rx_Buf,8);
						if(Back[3] == 0x41)           //���ճɹ�
						{
							LED1 = ~LED1;             //LED1��ת
							SYN7318_Get_String(Back,3);  //����ʶ������ش����
							if(Back[0] == 0xfc)        //֡ͷ�ж�
							{
								LED2 = ~LED2;
								SYN7318_Get_String(ASR,Back[2]);//���ջش�����
								
								switch(ASR[0])
							{
									case 0x01:
									{
										
										switch(ASR[3])
										{
											case 0x00:  // ��һ����һ�㲻����ʶ��
											{
												//	SYN_TTS("������");
												SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													break;
											}
											case 0x01:  // ǰ��
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													
													Go_Test( 50,50);
													SYN_TTS("С��ǰ�������");
													break;
											}
											case 0x02:
											{
												SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ�� 
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													
													Back_Test(50,50);
													SYN_TTS("С����������ɣ��ȴ���һ��ָ��");
													break;
											}
											case 0x03:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��;
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
												  
												  Left_Test( 80);
													SYN_TTS("С����ת��ɣ��ȴ���һ��ָ��");
													Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													Bzuozhuan();
													Bzuozhuan();
													xunji();
													zuozhuan();
													xunji();
													qianjingMP(10);
													break;
											}
											case 0x04:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
												   
													 Right_Test( 80);
													SYN_TTS("С����ת��ɣ��ȴ���һ��ָ��");
													Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													xunji();
													zuozhuan();
													xunji();
													qianjingMP(10);
													break;
											}
											case 0x05:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													
													Stop_Test();
													SYN_TTS("��ͣ�����ȴ���һ��ָ��");
													break;
											}
											case 0x06:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Track_Test( 50);
														SYN_TTS("Ѱ������ɣ��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x07:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													 SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
														Left_Test( 80);
													SYN_TTS("��ֹ��ת����ɣ��ȴ���һ��ָ��");
														Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													Bzuozhuan();
													Bzuozhuan();
													xunji();
													zuozhuan();
													xunji();
													qianjingMP(10);
													}
													break;
											}
											case 0x08:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													 SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( DZ_K ,8);  // ������բ
												   SYN_TTS("��բ�Ѵ򿪣��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x09:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( DZ_G ,8);  // ������բ
													SYN_TTS("��բ�ѹرգ��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x0a:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													SYN7318_Get_String(Back,4);
												 if(Back[3] == 0x41)
													{
														Right_Test( 80);
													SYN_TTS("��ֹ��ת��ɣ��ȴ���һ��ָ��");
														Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													xunji();
													zuozhuan();
													xunji();
													qianjingMP(10);
													}														
													break;
											}
											case 0x0b:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( SMG_JSK ,8);  // ����ܼ�ʱ
													SYN_TTS("��ʱϵͳ�Ѵ򿪣��ȴ���һ��ָ��");
													}															
													break;
											}
											case 0x0c:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												 if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( SMG_JSG ,8);  // ����ܹر�
													SYN_TTS("��ʱϵͳ�ѹرգ��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x0d:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( SMG_JL ,8);  // �������ʾ����
													SYN_TTS("LED��ʾ��������ɣ��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x0e:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														  LED_L =0;  // �����
													SYN_TTS("�����ʾ���Ѵ򿪣��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x0f:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														  LED_R =0;  // ���ҵ�
													SYN_TTS("�Ҳ���ʾ���Ѵ򿪣��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x10:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														 LED_L =1;  // �ر���ʾ��
														 LED_R =1;
													SYN_TTS("��ʾ���ѹرգ��ȴ���һ��ָ��");
													}

													break;
											}
											case 0x11:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														 BEEP =0;  //��������
													 SYN_TTS("�������Ѵ򿪣��ȴ���һ��ָ��");
														 delay_ms(1000);
														 BEEP =1;
													}
													break;
											}
											case 0x12:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														 BEEP =1;  //�ط�����
													 SYN_TTS("�������ѹرգ��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x13:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)// ��ʾ����
													{
														 Transmition(CP_SHOW1,6);
														 delay_ms(500);
														 Transmition(CP_SHOW2,6);
														
													 SYN_TTS("��������ʾ���ȴ���һ��ָ��");
													}
													break;
											}
											case 0x14:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)// �����
													{
														 Transmition(H_SD,4);
														
													 SYN_TTS("����ŷ�ϵͳ�Ѵ򿪣��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x15:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)// ͼƬ��ҳ��
													{
														 Transmition(H_S,4);
														
													 SYN_TTS("ͼƬ��ҳ��ɣ��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x16:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)// ���⵵λ+1
													{
														 Transmition(H_1,4);
														
													 SYN_TTS("��[=tiao2]�⵵[=dang3]λ�Ѽ�1���ȴ���һ��ָ��");
													}
													break;
											}
											case 0x17:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)// ��������
													{																																
													 SYN_TTS("��ӭʹ�ñ����ٿ���[=rong2]����������С��");
													}
													break;
											}
											case 0x18:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)// ԭ�ص�ͷ
													{																																
													 SYN_TTS("��ʼԭ�ص�ͷ�����Ժ�");
														 Left_Test( 80); 
														 while(L_Flag);
														Left_Test( 80); 
														 while(L_Flag);
														SYN_TTS("ԭ�ص�ͷ����ɣ��ȴ���һ��ָ��");
														Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													xunji();
													zuozhuan();
														xunji();
														zuozhuan();
														xunji();
														qianjingMP(10);
														xunji();
														qianjingMP(10);
													}
													break;
											}
											case 0x19:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													SYN_TTS("�õģ�����������������");
												  Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													break;
											}
											case 0x1A:  // С��ǰ��
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
													Go_Test( 80,10);
													SYN_TTS(" С��ǰ������ɣ��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x1B:   //  С������
											{
												SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ�� 
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
													
													  Back_Test(80,10);
													  SYN_TTS("С����������ɣ��ȴ���һ��ָ��");	
													}
													break;
											}
											case 0x1C:   //  С����ת
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��;
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
												  
													Left_Test( 80);
														SYN_TTS("С����ת����ɣ��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x1D:  // С����ת
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
												   
													  Right_Test( 80);
														SYN_TTS("С����ת����ɣ��ȴ���һ��ָ��");
													}
													break;
											}
											case 0x1E:  // С��ֹͣ
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
													
													  Stop_Test();
														SYN_TTS("��ͣ�����ȴ���һ��ָ��");
													}
													break;
											}
											case 0x1F:   //  С��ѭ��
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Track_Test( 50);
														SYN_TTS("С��Ѱ������ɣ��ȴ���һ��ָ��");
													}
													break;
											}	
				  case 0x20:   //  ����ת��
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Right_Test( 80);
														SYN_TTS("��ת������ɣ��ȴ���һ��ָ��");
													}
													break;
											}														
				  case 0x21:   //  ��ֹ��ת
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Left_Test( 80);
														SYN_TTS("��ֹ��ת��ִ����ת����ɣ��ȴ���һ��ָ��");
													}
													break;
												}
				  case 0x22:   //  ����ת��
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Left_Test( 80);
														SYN_TTS("��ת������ɣ��ȴ���һ��ָ��");
													}
													break;
											}														
				  case 0x23:   //  ��ֹ��ת
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Left_Test( 80);
														SYN_TTS("��ֹ��ת��ִ����ת����ɣ��ȴ���һ��ָ��");
													}
													break;													
											}
											default:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
													SYN_TTS("�Բ�����û����");
													break;
											}
										}
				SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
										break;
									}
									case 0x02: //ʶ��ɹ���������ID�ţ�
									{
										SYN_TTS("�Բ�����û����");
										SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
										break;
									}
									case 0x03://�û�������ʱ
									{
										SYN_TTS("�������ߣ���ʱΪ������");
										Ysn7813_flag = 0;
										Ysn7813_flag_cs = 0;
										SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
										break;
									}
									case 0x04:
									{
										SYN_TTS("�밲�����鷳����˵һ��");
										SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
										break;
									}
									case 0x05:
									{
										SYN_TTS("�Բ�������˵һ��");
										SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
										break;
									}
									case 0x06:
									{
										SYN_TTS("ʶ���ڲ�����");
										SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
										break;
									}
									case 0x07:
									{
										SYN_TTS("�Բ�������˵һ��");
										SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
										break;
									}
									default:
									{
										SYN_TTS("����");
										SYN7318_Put_String(Stop_ASR_Buf,4); //ֹͣ����ʶ��
										break;
									}
							}
							}
						}
					}
					/*************************************************/
					SYN7318_Put_String(Stop_Wake_Up,4);//����ֹͣ����ָ��
				}
				else                          //�����ڲ�����
				{
					LED3 =~LED3;
				}
			}
		}	
		else 
		{
			Zigb_Rx_Buf[0]=0x55;
			Zigb_Rx_Buf[1]=0x06;
			Zigb_Rx_Buf[2]=0x10;
			Zigb_Rx_Buf[3]=0x01;
			Zigb_Rx_Buf[4]=0x00;
			Zigb_Rx_Buf[5]=0x00;
			Zigb_Rx_Buf[6]=0x11;
			Zigb_Rx_Buf[7]=0xBB;
			Send_ZigbeeData_To_Fifo(Zigb_Rx_Buf,8);
			SYN7318_Get_String(Back,4);   //���շ�����Ϣ
		}
		}
		
	}
}
