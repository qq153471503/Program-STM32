#ifndef __BH1750_H
#define __BH1750_H	 
#include "sys.h"


//NG1750�˿ڶ���
#define  GPIO_MODE    0  // 0 OD�� ���ⲿ����������¿ɲ��л�����

#if  GPIO_MODE
//SDA ��������	   //��Ҫʹ��GPIOB->CRL|=3<<7,��ȻIIC�᲻������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}	 

#else 
#define SDA_IN()   //  ��������ΪOD�ţ�����Ҫ�л����� 
#define SDA_OUT()

#endif

//IO��������	 
#define IIC_SCL    PBout(6) //SCL 12
#define IIC_SDA    PBout(7) //SDA 11	 
#define READ_SDA   PBin(7)  //����SDA 
#define ADDR PBout(5)   //PB5	



//IIC���в�������
void IIC_Init(void);
void BH1750_Init(void);
void conversion(unsigned int temp_data);
void  Single_Write_BH1750(u8 REG_Address);            //����д������
u8 Single_Read_BH1750(u8 REG_Address);                //������ȡ�ڲ��Ĵ�������
void  Multiple_Read_BH1750(void);                     //�����Ķ�ȡ�ڲ��Ĵ�������
//------------------------------------

void BH1750_Start(void);                    //��ʼ�ź�
void BH1750_Stop(void);                     //ֹͣ�ź�
void BH1750_SendACK(u8 ack);           //Ӧ��ACK
u8  BH1750_RecvACK(void);                  //��ack
void BH1750_SendByte(u8 dat);         //IIC�����ֽ�д
u8 BH1750_RecvByte(void);                 //IIC�����ֽڶ�
unsigned int Dispose(void);
		 				    
void Bh1750_Test(void);								
								
#endif
