#ifndef __BH1750_H
#define __BH1750_H	 
#include "sys.h"


//NG1750端口定义
#define  GPIO_MODE    0  // 0 OD门 在外部有上拉情况下可不切换方向

#if  GPIO_MODE
//SDA 方向设置	   //不要使用GPIOB->CRL|=3<<7,不然IIC会不起作用
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}	 

#else 
#define SDA_IN()   //  总线设置为OD门，不需要切换方向 
#define SDA_OUT()

#endif

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL 12
#define IIC_SDA    PBout(7) //SDA 11	 
#define READ_SDA   PBin(7)  //输入SDA 
#define ADDR PBout(5)   //PB5	



//IIC所有操作函数
void IIC_Init(void);
void BH1750_Init(void);
void conversion(unsigned int temp_data);
void  Single_Write_BH1750(u8 REG_Address);            //单个写入数据
u8 Single_Read_BH1750(u8 REG_Address);                //单个读取内部寄存器数据
void  Multiple_Read_BH1750(void);                     //连续的读取内部寄存器数据
//------------------------------------

void BH1750_Start(void);                    //起始信号
void BH1750_Stop(void);                     //停止信号
void BH1750_SendACK(u8 ack);           //应答ACK
u8  BH1750_RecvACK(void);                  //读ack
void BH1750_SendByte(u8 dat);         //IIC单个字节写
u8 BH1750_RecvByte(void);                 //IIC单个字节读
unsigned int Dispose(void);
		 				    
void Bh1750_Test(void);								
								
#endif
