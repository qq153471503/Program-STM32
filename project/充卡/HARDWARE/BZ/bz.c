#include "stm32f10x.h"  //����
#include "sys.h"					
#include "usart.h"		
#include "delay_drv.h"
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

u8 guangdang;

u16 TableDis[50];	//������Ϣ����
u16 TableLight[10];//��ǿ����Ϣ����
u8 Table[8]={0x55,0xaa,0x00,0x00,0x00,0x00,0x00,0xbb};
u8 Kahao[16]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
u8 Tabyuyin[24]={"RFID���еĵ�  �������Ϊ"};
u8 HW[6];
///////////////////////////����־��///////////////////////////////////////////
void ceju(void);
void Distance(void);	//���������Ժ���
void CLight(void);			//���ǿ�����ҵ���
void erweima(void);		//����ɨ���ά��ָ����ȴ�ƽ�巢��ɨ�����
void Chepai(void);		//����ɨ�賵������
void Tuxing(void);		//����ɨ��ͼ������
void LTDisplay(void);	//����������ʾ��־��
void Shibiefangxiang(void);	//ʶ����
void Cixuanfu(void);		//�򿪴�������־��
void duka(void);				//��������
void TFTOpenJS(void);		//TFT�򿪼�ʱģʽ
void TFTCloseJS(void);	//TFT�رռ�ʱģʽ
void TFTDown(void);			//TFT�·�
void TFTUp(void);				//TFT�Ϸ�
void ETCopen(void);
//////////////////////////��������//////////////////////////////////////////
void zuozhuan(void);	//////////////////��ת
void youzhuan(void);	//////////////////��ת
void Bzuozhuan(void);	//////////////////ԭ����ת
void Byouzhuan(void);	//////////////////ԭ����ת
void qianjinMP(u8 mp);//////////////////ǰ��MP
void houtuiMP(u8 mp);	//////////////////����MP
void qianxun(u8 mp);
////////////////////////wifi��zigbeeͨ��//////////////////////////////////////
void WifiSend(void);
void ZigbeeSend(void);
///////////////////////////zigbee����/////////////////////////////////////////
void OpenJS(void);
void CloseJS(void);
void OpenDZ(void);
void CloseDZ(void);
void OpenBJQ(void);
///////////////////////////�ӳ�����///////////////////////////////////////////
void congche(void);
////////////////////////////////////////////////////////////////////////////////
void ceju(void)
{
	u8 j,jj,bai,shi;
	u16 tempp,pingjun;
	unsigned long add=0;
	
	for(j=0;j<50;j++)
	{
		tran();
		delay_ms(30);
		if(dis>10&&dis<6200)
		{
			TableDis[j]=dis;
//			U1SendChar(dis/100+0x30);
//			U1SendChar((dis%100)/10+0x30);
//			U1SendChar((dis%100)%10+0x30);
//			U1SendChar(0x0d);
		}
		else
			j-=1;
		
	}
	for(j=0;j<49;j++)
	{
		for(jj=j+1;jj<50;jj++)
		{
			if(TableDis[j]>TableDis[jj])
			{
				tempp=TableDis[j];
				TableDis[j]=TableDis[jj];
				TableDis[jj]=tempp;
			}
		}
	}
	tempp=0;
	for(j=23;j<28;j++)
	{
		add+=TableDis[j];
	}
	pingjun=(u16)(add/5);
	bai=(u8)(pingjun/100);
	shi=(u8)(pingjun%100);
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x04;
	Zigb_Rx_Buf[2]=0x04;
	Zigb_Rx_Buf[3]=0x00;
	Zigb_Rx_Buf[4]=0x00+bai;
	Zigb_Rx_Buf[5]=0x00+shi;
	Zigb_Rx_Buf[6]=((0x04+bai+shi)%0xff);
	Zigb_Rx_Buf[7]=0xbb;
	ZigbeeSend();
	delay_ms(500);
	if(pingjun<750)
	{
		biao=2;
		
	//	Track();
	}
	else 
	{	
		biao=1;	
	}

}
void Distance(void)   // ��������ຯ��
{
	u8 bai,shi,ge,j,jj,shi1,a;
	u16 tempp,pingjun;
	unsigned long add=0;
	for(j=0;j<50;j++)
	{
		tran();
		delay_ms(30);
//		if(dis>50&&dis<500)
//		{
			TableDis[j]=dis;
//		}
//		else 
//			
//				j-=1;
	}
	for(j=0;j<49;j++)
	{
		for(jj=j+1;jj<50;jj++)
		{
			if(TableDis[j]>TableDis[jj])
			{
				tempp=TableDis[j];
				TableDis[j]=TableDis[jj];
				TableDis[jj]=tempp;
			}
		}
	}
	tempp=0;
	for(j=21;j<26;j++)
	{
		add+=TableDis[j];
	}
	pingjun=(u16)(add/5);
	pingjun=pingjun/10;
	pingjun=pingjun+0.5;
	pingjun=(int)pingjun;
	pingjun=pingjun*10;
	if(pingjun<=100)
	{
		pingjun=pingjun;
	}
	else if(pingjun>100&&pingjun<=200)
	{
		pingjun=pingjun+10;
	}
	else if(pingjun>200&&pingjun<=300)
	{
		pingjun=pingjun+20;
	}
	else if(pingjun>300&&pingjun<=400)
	{
		pingjun=pingjun+30;
	}
	else if(pingjun>400)
	{
		pingjun=pingjun+40;
	}
	bai=(u8)(pingjun/100);
	shi=(u8)(pingjun%100);
	shi1=(u8)(pingjun%100/10);
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x04;
	Zigb_Rx_Buf[2]=0x04;
	Zigb_Rx_Buf[3]=0x00;
	Zigb_Rx_Buf[4]=0x00+bai;
	Zigb_Rx_Buf[5]=0x00+shi;
	Zigb_Rx_Buf[6]=((0x04+bai+shi)%0xff);
	Zigb_Rx_Buf[7]=0xbb;
	ZigbeeSend();
	delay_ms(500);
	Table[3]=0x01;
	Table[4]=pingjun%256;
	Table[5]=pingjun/256;
	WifiSend();
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLight(void)	//////////////////////////�����ǿ�ȣ�������
{
	u8 a,c,jj;
	unsigned int b,tempp;
	unsigned int x1,x2,x3,x4;
	unsigned int add=0;
	u8 num=1;
	u16 j=0;
	delay_ms(500);
	//��һ����
	for(a=0;a<6;a++)
	{
		b=Dispose();
		if(b<0x4000)
		{  
				TableLight[a]=b;

		}
		else
		a-=1;
		
	}
	for(a=0;a<5;a++)
	{
		for(jj=a+1;jj<6;jj++)
		{
			if(TableLight[a]>TableLight[jj])
			{
				tempp=TableLight[a];
				TableLight[a]=TableLight[jj];
				TableLight[jj]=tempp;
			}
		}
	}
	tempp=0;
	for(a=2;a<4;a++)
	{
		add+=TableLight[a];
	}
	x1=add/2;
	add=0;
	Transmition(H_1,4);
	//�ڶ�����
	delay_ms(300);
		for(a=0;a<6;a++)
	{
		b=Dispose();
		if(b<0x4000)
		{
			TableLight[a]=b;
		}
		else 
			a-=1;
	}
	for(a=0;a<5;a++)
	{
		for(jj=a+1;jj<6;jj++)
		{
			if(TableLight[a]>TableLight[jj])
			{
				tempp=TableLight[a];
				TableLight[a]=TableLight[jj];
				TableLight[jj]=tempp;
			}
		}
	}
	tempp=0;
	for(a=2;a<4;a++)
	{
		add+=TableLight[a];
	}
	x2=add/2;
	add=0;
	Transmition(H_1,4);
	delay_ms(300);
	//��������
		for(a=0;a<6;a++)
	{
		b=Dispose();
		if(b<0x4000)
		{
			TableLight[a]=b;
		}
		else 
			a-=1;
	}
	for(a=0;a<5;a++)
	{
		for(jj=a+1;jj<6;jj++)
		{
			if(TableLight[a]>TableLight[jj])
			{
				tempp=TableLight[a];
				TableLight[a]=TableLight[jj];
				TableLight[jj]=tempp;
			}
		}
	}
	tempp=0;
	for(a=2;a<4;a++)
	{
		add+=TableLight[a];
	}
	x3=add/2;
	add=0;
	Transmition(H_1,4);
	delay_ms(300);
	//���ĸ���
		for(a=0;a<6;a++)
	{
		b=Dispose();
		if(b<0x4000)
		{
			TableLight[a]=b;
		}
		else 
			a-=1;
	}
	for(a=0;a<5;a++)
	{
		for(jj=a+1;jj<6;jj++)
		{
			if(TableLight[a]>TableLight[jj])
			{
				tempp=TableLight[a];
				TableLight[a]=TableLight[jj];
				TableLight[jj]=tempp;
			}
		}
	}
	tempp=0;
	for(a=2;a<4;a++)
	{
		add+=TableLight[a];
	}
	x4=add/2;
	add=0;
	if(x1>x2)
	{
		num++;
	}
	if(x1>x3)
	{
		num++;
	}
	if(x1>x4)
	{
		num++;
	}
	Send_Debug_Info("Guang1\n",6);
	Wifi_Rx_flag =0;
	Table[3]=0x02;
	WifiSend();
	while(Wifi_Rx_flag ==0);  // wifi ���ձ��
	Wifi_Rx_flag=0;
	switch(num)
	{
		case 1 :
				guangdang=1;
				if(Wifi_Rx_Buf[5]==0x07)
				{
					Transmition(H_1,4);
					Send_Debug_Info("41A\n",6);

				}
				if(Wifi_Rx_Buf[5]==0x08)
				{
					Transmition(H_2,4);
					Send_Debug_Info("42A\n",6);
					
				}
				if(Wifi_Rx_Buf[5]==0x09)
				{
					Transmition(H_3,4);
					Send_Debug_Info("43A\n",6);
					
				}
				if(Wifi_Rx_Buf[5]==0x10)
				{
					
					Send_Debug_Info("44A\n",6);
				}
				break;
		case 2 :
				guangdang=2;
				if(Wifi_Rx_Buf[5]==0x07)
				{
					
					Send_Debug_Info("11A\n",6);
				}
				if(Wifi_Rx_Buf[5]==0x08)
				{
					Transmition(H_1,4);
					Send_Debug_Info("12A\n",6);
				}
				if(Wifi_Rx_Buf[5]==0x09)
				{
					Transmition(H_2,4);
					Send_Debug_Info("13A\n",6);
				}
				if(Wifi_Rx_Buf[5]==0x10)
				{
					Transmition(H_3,4);
					Send_Debug_Info("14A\n",6);
				}
				break;
		case 3 :
				guangdang=3;
				if(Wifi_Rx_Buf[5]==0x07)
				{
					Transmition(H_3,4);
					Send_Debug_Info("21A\n",6);
				}
				if(Wifi_Rx_Buf[5]==0x08)
				{
					
					Send_Debug_Info("22A\n",6);
				}
				if(Wifi_Rx_Buf[5]==0x09)
				{
					Transmition(H_1,4);
					Send_Debug_Info("23A\n",6);
				}
				if(Wifi_Rx_Buf[5]==0x10)
				{
					Transmition(H_2,4);
					Send_Debug_Info("24A\n",6);
				}
				break;
		case 4 :
				guangdang=4;
				if(Wifi_Rx_Buf[5]==0x07)
				{
					Transmition(H_2,4);
					Send_Debug_Info("31A\n",6);
				}
				if(Wifi_Rx_Buf[5]==0x08)
				{
					Transmition(H_3,4);
					Send_Debug_Info("32A\n",6);
				}
				if(Wifi_Rx_Buf[5]==0x09)
				{
					
					Send_Debug_Info("33A\n",6);
				}
				if(Wifi_Rx_Buf[5]==0x10)
				{
					Transmition(H_1,4);
					Send_Debug_Info("34A\n",6);
				}
				break;
	}
}
void erweima(void)//ɨ���ά��
{
	Wifi_Rx_flag =0;
	Table[3]=0x10;	
	WifiSend();
	while(Wifi_Rx_flag ==0);  // wifi ���ձ��
	 Wifi_Rx_flag =0;
}

void erweima1(void)
{
	Wifi_Rx_flag=0;
	Table[3]=0x11;
	WifiSend();
	while(Wifi_Rx_flag ==0);  // wifi ���ձ��
	 Wifi_Rx_flag =0;
	if(Wifi_Rx_Buf[0]==0xFD)  // ���յ��������������ݣ�ֱ��ת��
	{
			send_data_zigbee( Wifi_Rx_Buf , (Wifi_Rx_num +1)); 
	}
	
}
void baojingqi(void)
{
	u8 a;
	Table[3]=0x08;
	WifiSend();
	while(Wifi_Rx_flag ==0);  // wifi ���ձ��
	 Wifi_Rx_flag =0;
	HW_K[0]=Wifi_Rx_Buf[2];
	HW_K[1]=Wifi_Rx_Buf[3];
	HW_K[2]=Wifi_Rx_Buf[4];
	HW_K[3]=Wifi_Rx_Buf[5];
	while(Wifi_Rx_flag ==0);  // wifi ���ձ��
	 Wifi_Rx_flag =0;
	HW_K[4]=Wifi_Rx_Buf[2];
	HW_K[5]=Wifi_Rx_Buf[3];
	delay_ms(100);
	for(a=0;a<8;a++)
	{
		Transmition(HW_K,6);
		delay_ms(200);	
	}
}

void Chepai(void)//ɨ�賵�ƺ�
{
	u8 i,a=0;
	for(i=0;i<16;i++)
	{
		if((READ_RFID[i]&0xFF)==0xFF)
			a++;
	}
	Wifi_Rx_flag=0;
	delay_ms(1000);
	delay_ms(1000);
	for(i=0;i<a;i++)
	{
		TFTDown();
		delay_ms(1000);
		delay_ms(1000);
	}
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	Table[3]=0x05;
	WifiSend();
	while(Wifi_Rx_flag==0);
	Wifi_Rx_flag=0;
	CP_SHOW1[2]=Wifi_Rx_Buf[2];
	CP_SHOW1[3]=Wifi_Rx_Buf[3];
	CP_SHOW1[4]=Wifi_Rx_Buf[4];
	CP_SHOW1[5]=Wifi_Rx_Buf[5];
	while(Wifi_Rx_flag==0);
	Wifi_Rx_flag=0;
	CP_SHOW2[2]=Wifi_Rx_Buf[2];
	CP_SHOW2[3]=Wifi_Rx_Buf[3];
}
void Tuxing(void)// ɨ��ͼ��
{
	Wifi_Rx_flag =0;
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	Table[3]=0x04;
	WifiSend();
	while(Wifi_Rx_flag ==0);  // wifi ���ձ��
	 Wifi_Rx_flag =0;
	tu1=Wifi_Rx_Buf[2]%6+2;
	tu2=Wifi_Rx_Buf[3]%4+1;
}
void LTDisplay(void)	//����������ʾ��־��
{
		u8 b;	
		delay_ms(1000);
//	for(b=0;b<3;b++)
//	{
		Transmition(CP_SHOW1,6);
		delay_ms(10);
		Transmition(CP_SHOW2,6);
//		delay_ms(2000);
//	}
}
void Shibiefangxiang(void)	//ʶ����
{	
	STOP();
	Wifi_Rx_flag=0;
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);;
	Table[3]=0x06;
	WifiSend();
	while(Wifi_Rx_flag==0);
	Wifi_Rx_flag=0;
	delay_ms(1000);
	switch(Wifi_Rx_Buf[5])
	{
		case 0x04:
							line=46;
							break;
		case 0x05:
							line=47;
							break;
		case 0x06:
							line=48;
							break;
	}
}

void Yuyinbobao(void)
{
		u8 a;
	Tabyuyin[12]=Kuai;
	Zigb_Rx_Buf[0]=0xFD;	
	Zigb_Rx_Buf[1]=0x00;
	Zigb_Rx_Buf[2]=0x1A;
	Zigb_Rx_Buf[3]=0x01;
	Zigb_Rx_Buf[4]=0x01;
	for(a=0;a<24;a++)
		Zigb_Rx_Buf[a+5]=Tabyuyin[a];
	send_data_zigbee(Zigb_Rx_Buf,29);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	Zigb_Rx_Buf[2]=0x12;
	for(a=0;a<16;a++)
	Zigb_Rx_Buf[a+5]=Card1[a];
	send_data_zigbee(Zigb_Rx_Buf,21);
}
void duka(void)
{
		u8 i;
		ka=1;
		if(ka<3)
		{
			while(!Kahao_flag&&Cishu<6)
			{
				Rc522(0,0);
				delay_ms(2);
			}
			Cishu=0;
			if(Kahao_flag==1)
			{
				Kahao_flag=0;
				if(!(contrast_data(Kahao,upKahao,16)))
				{
					if(ka==1)
					{
						Cishu=0;
						while(!Kahao_flag&&Cishu<6)
						{
							Rc522(5,0);
							delay_ms(2);
						}
						if(Read_flag==1)
						{
							Read_flag=0;							
							for(i=0;i<16;i++)
								Card1[i]=READ_RFID[i];
							send_data_wifi(Card1,16);
						}
						for(i=0;i<16;i++)
						{
							Kahao[i]=upKahao[i];
						}						
					}
					if(ka==2)
					{
						Cishu=0;
						while(!Kahao_flag&&Cishu<6)
						{
							Rc522(5,0);
							delay_ms(2);
						}
						if(Read_flag==1)
						{
							Read_flag=0;							
							for(i=0;i<16;i++)
								Card2[i]=READ_RFID[i];
							send_data_wifi(Card2,16);								
						}
						for(i=0;i<16;i++)
						{
							Kahao[i]=upKahao[i];
						}						
					}
					ka++;
				}	
				}			
		}			
}

////////////////////////////��������///////////////////////////////////////////////////////////////////////////////////////
void xunji1(void)
{
	Car_Spend = 15;
	Stop_Flag=0;
	Track_Flag=1;MP=0; 					  
	Host_Open_UpTrack( Up_Track_Speed );  // ����Ѱ�������ϴ�
	delay_ms(100); // �ȴ�һ��Ѱ�������ϴ�
	TIM_Cmd(TIM2, ENABLE);
	while(Stop_Flag!=0x01);
}
void xunji(void)
{
	Car_Spend = 40;
	Track_Flag=1;MP=0; Stop_Flag=0;					  
	Host_Open_UpTrack( Up_Track_Speed );  // ����Ѱ�������ϴ�
	delay_ms( 100); // �ȴ�һ��Ѱ�������ϴ�
	TIM_Cmd(TIM2, ENABLE);
	while(Stop_Flag!=0x01);
}
void zuozhuan(void)
{
	qianjingMP(13);
	MP=0;L_Flag=1;	Stop_Flag=0;
	Car_Spend = 80;
	Host_Open_UpTrack( Up_Track_Speed );
	Control(-Car_Spend,Car_Spend);
	while(Stop_Flag!=0x02);

}

void youzhuan(void)
{	
	qianjingMP(15);
	MP=0;R_Flag=1;	Stop_Flag=0;
	Car_Spend = 	80;
	Host_Open_UpTrack( Up_Track_Speed );
	Control(Car_Spend,-Car_Spend);
	while(Stop_Flag!=0x02);
}
void Bzuozhuan(void)
{
	MP=0;L_Flag=1;	Stop_Flag=0;
	Car_Spend = 80;
	Host_Open_UpTrack( Up_Track_Speed );
	Control(-Car_Spend,Car_Spend);
	while(Stop_Flag!=0x02);

}

void Byouzhuan(void)
{	
	MP=0;R_Flag=1;	Stop_Flag=0;
	Car_Spend = 	80;
	Host_Open_UpTrack( Up_Track_Speed );
	Control(Car_Spend,-Car_Spend);
	while(Stop_Flag!=0x02);
}
void sishiwu(void)
{
	Control(80,-80);
	delay_ms(450);
	STOP();
}

void qianjinMP(u8 mp)
{
	MP=0;G_Flag=1;Stop_Flag=0;tempMP=mp;
	Control(80,80);
	while(Stop_Flag!=0x03);
}
void qianxun(u8 mp)
{
	Car_Spend = 40;
	Track_Flag=1;MP=0; Stop_Flag=0;					  
	Host_Open_UpTrack( Up_Track_Speed );  // ����Ѱ�������ϴ�
	delay_ms( 100); // �ȴ�һ��Ѱ�������ϴ�
	TIM_Cmd(TIM2, ENABLE);
	while(MP!=mp);
}
void qianjingMP(u8 mp)
{
	MP=0;G_Flag=1;Stop_Flag=0;tempMP=mp;
	Control(40,40);
	while(Stop_Flag!=0x03);
}
void houtuiMP(u8 mp)
{
	MP=0;B_Flag=1;Stop_Flag=0;
	tempMP=mp;
	Control(-40,-40);
	while(Stop_Flag!=0x03);
}
////////////////////////wifi��zigbeeͨ��//////////////////////////////////////
void WifiSend(void)
{
	send_data_wifi(Table,8);	
}
void ZigbeeSend(void)
{
	send_data_zigbee(Zigb_Rx_Buf,8);	//��ƽ�巢������
}
////////////////////zigbee����/////////////////////////////////
void OpenJS(void)		//���Ƽ�ʱ������
{
	
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x04;
	Zigb_Rx_Buf[2]=0x03;
	Zigb_Rx_Buf[3]=0x01;
	Zigb_Rx_Buf[4]=0x00;
	Zigb_Rx_Buf[5]=0x00;
	Zigb_Rx_Buf[6]=(0x04%0xFF);
	Zigb_Rx_Buf[7]=0xBB;
	ZigbeeSend();

}
void CloseJS(void)		//���Ƽ�ʱ���ر�
{
	
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x04;
	Zigb_Rx_Buf[2]=0x03;
	Zigb_Rx_Buf[3]=0x00;
	Zigb_Rx_Buf[4]=0x00;
	Zigb_Rx_Buf[5]=0x00;
	Zigb_Rx_Buf[6]=(0x03%0xFF);
	Zigb_Rx_Buf[7]=0xBB;
	ZigbeeSend();

}
void OpenDZ(void)		//���Ƶ�բ����
{
		u8 i;
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x03;
	Zigb_Rx_Buf[2]=0x01;
	Zigb_Rx_Buf[3]=0x01;
	Zigb_Rx_Buf[4]=0x00;
	Zigb_Rx_Buf[5]=0x00;
	Zigb_Rx_Buf[6]=(0x02%0xFF);
	Zigb_Rx_Buf[7]=0xBB;
	for(i=0;i<3;i++)
	ZigbeeSend();
}
void CloseDZ(void)		//���Ƶ�բ�ر�
{
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x03;
	Zigb_Rx_Buf[2]=0x01;
	Zigb_Rx_Buf[3]=0x02;
	Zigb_Rx_Buf[4]=0x00;
	Zigb_Rx_Buf[5]=0x00;
	Zigb_Rx_Buf[6]=(0x03%0xFF);
	Zigb_Rx_Buf[7]=0xBB;
	ZigbeeSend();	
}
void OpenBJQ(void)
{
	u8 i,j,temp;
	for(i=0;i<16;i++)
	{
		for(j=i+1;j<17;j++)
		{
			if(READ_RFID[i]>READ_RFID[j])
			{
				temp=READ_RFID[i];
				READ_RFID[i]=READ_RFID[j];
				READ_RFID[j]=temp;
			}
		}
	}
	HW[0]=READ_RFID[0];
	HW[1]=READ_RFID[2];
	HW[2]=READ_RFID[5];
	HW[3]=READ_RFID[9];
	HW[4]=0xDE;
	HW[5]=0x92;
	Transmition(HW,6);
	Wifi_Rx_Buf[0]=0x55;
	Wifi_Rx_Buf[1]=0xAA;
	Wifi_Rx_Buf[3]=0x07;
	Wifi_Rx_Buf[4]=READ_RFID[0];
	Wifi_Rx_Buf[5]=READ_RFID[1];
	Wifi_Rx_Buf[6]=READ_RFID[2];
	Wifi_Rx_Buf[7]=READ_RFID[3];
	Wifi_Rx_Buf[8]=READ_RFID[4];
	Wifi_Rx_Buf[9]=READ_RFID[5];
	send_data_wifi(Wifi_Rx_Buf,10);	
	Wifi_Rx_flag =0;
	while(Wifi_Rx_flag==0);	
	Wifi_Rx_flag =0;
	send_data_zigbee( Wifi_Rx_Buf ,8);		
	while(Wifi_Rx_flag==0);
	Wifi_Rx_flag=0;
	send_data_zigbee( Wifi_Rx_Buf ,8);			
}
void Cixuanfu(void)	//���ƴ�������־�￪��
{
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x0a;
	Zigb_Rx_Buf[2]=0x01;
	Zigb_Rx_Buf[3]=0x01;
	Zigb_Rx_Buf[4]=0x00;
	Zigb_Rx_Buf[5]=0x00;
	Zigb_Rx_Buf[6]=(0x02%0xFF);
	Zigb_Rx_Buf[7]=0xBB;
	ZigbeeSend();
}
void TFTOpenJS(void)	//TFT�򿪼�ʱ
{
		u8 i;
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x0b;
	Zigb_Rx_Buf[2]=0x30;
	Zigb_Rx_Buf[3]=0x01;
	Zigb_Rx_Buf[4]=0x00;
	Zigb_Rx_Buf[5]=0x00;
	Zigb_Rx_Buf[6]=0x31;
	Zigb_Rx_Buf[7]=0xBB;
	for(i=0;i<3;i++)
	{
		ZigbeeSend();
		delay_ms(200);
	}
}
void TFTCloseJS(void)	//TFT�رռ�ʱ
{
		u8 i;
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x0b;
	Zigb_Rx_Buf[2]=0x30;
	Zigb_Rx_Buf[3]=0x00;
	Zigb_Rx_Buf[4]=0x00;
	Zigb_Rx_Buf[5]=0x00;
	Zigb_Rx_Buf[6]=0x30;
	Zigb_Rx_Buf[7]=0xBB;
	for(i=0;i<3;i++)
	ZigbeeSend();
}
void TFTDown(void)		//TFT�·�
{
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x0b;
	Zigb_Rx_Buf[2]=0x10;
	Zigb_Rx_Buf[3]=0x02;
	Zigb_Rx_Buf[4]=0x00;
	Zigb_Rx_Buf[5]=0x00;
	Zigb_Rx_Buf[6]=0x12;
	Zigb_Rx_Buf[7]=0xBB;
	ZigbeeSend();
}
void TFTUp(void)			//TFT�Ϸ�
{
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x0b;
	Zigb_Rx_Buf[2]=0x10;
	Zigb_Rx_Buf[3]=0x01;
	Zigb_Rx_Buf[4]=0x00;
	Zigb_Rx_Buf[5]=0x00;
	Zigb_Rx_Buf[6]=0x11;
	Zigb_Rx_Buf[7]=0xBB;
	ZigbeeSend();
}
void ETCopen(void)	//����ETC����
{
		Zigb_Rx_Buf[4]=0x00;
		while(Zigb_Rx_Buf[4]!=0x06);
}
////////////////////////////�ӳ�����//////////////////////////
void congche(void)	//�ӳ��˶�	
{
		u8 i;
	CP_SHOW1[2]=0x47;
	CP_SHOW1[3]=0x48;
	CP_SHOW1[4]=0x49;
	CP_SHOW1[5]=0x4a;
	CP_SHOW2[2]=0x4b;
	CP_SHOW2[3]=0x4C;
	Zigbee_Rx_flag=0;
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x02;
	Zigb_Rx_Buf[2]=0x01;
	Zigb_Rx_Buf[3]=CP_SHOW1[2];
	Zigb_Rx_Buf[4]=CP_SHOW1[3];
	Zigb_Rx_Buf[5]=CP_SHOW1[4];
	Zigb_Rx_Buf[6]=(0x01+CP_SHOW1[2]+CP_SHOW1[3]+CP_SHOW1[4]%0xFF);
	Zigb_Rx_Buf[7]=0xBB;
	ZigbeeSend();	
	delay_ms(1000);
	Zigb_Rx_Buf[0]=0x55;
	Zigb_Rx_Buf[1]=0x02;
	Zigb_Rx_Buf[2]=0x02;
	Zigb_Rx_Buf[3]=CP_SHOW1[5];
	Zigb_Rx_Buf[4]=CP_SHOW2[2];
	Zigb_Rx_Buf[5]=CP_SHOW2[3];
	Zigb_Rx_Buf[6]=(0x01+CP_SHOW1[5]+CP_SHOW2[2]+CP_SHOW2[3]%0xFF);
	Zigb_Rx_Buf[7]=0xBB;
	ZigbeeSend();	

}

