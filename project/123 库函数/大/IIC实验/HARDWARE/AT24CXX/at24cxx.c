#include "at24cxx.h"
//��ʼ������
void AT24CXX_Init(void)
{
	IIC_Init();
}
//дһ���ֽ�
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
//��һ���ֽ�
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
//���
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
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 
//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
