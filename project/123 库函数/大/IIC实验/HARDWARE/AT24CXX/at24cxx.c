#include "at24cxx.h"
//初始化函数
void AT24CXX_Init(void)
{
	IIC_Init();
}
//写一个字节
void AT24CXX_WriteOneByte(u16 addr,u8 data)
{
	IIC_Start();
	IIC_Send_Byte(0XA0);
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
}
//读一个字节
u8 AT24CXX_ReadOneByte(u16 addr)
{
	u8 temp = 0;
	IIC_Start();
	IIC_Send_Byte(0XA0);
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0XA1);
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop();
	return temp;
}
//检查
u8 AT24CXX_Check(void)
{
	u8 flag;
	flag = AT24CXX_ReadOneByte(255);
	if(flag == 0x55)
		return 0;
	else
	{
		AT24CXX_WriteOneByte(255,0x55);
		flag = AT24CXX_ReadOneByte(255);
		
		if(flag == 0x55)
			return 0;
	}
	return 1;
}
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 
//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
