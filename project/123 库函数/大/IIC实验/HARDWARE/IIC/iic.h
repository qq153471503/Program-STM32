#ifndef _IIC_H
#define _IIC_H

#include "sys.h"

#define SET_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SET_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

#define SCL PBout(10)
#define SDA_IN PBin(11)
#define SDA PBout(11)
 
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(u8 ack);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

void IIC_WriteOneByte(u8 add,u8 data);
u8 IIC_ReadOneByte(u8 add,u8 data);
#endif


