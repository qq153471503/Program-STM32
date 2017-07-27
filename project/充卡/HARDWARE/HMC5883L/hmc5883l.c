#include <math.h>
#include "sys.h"
#include "delay.h"
#include "hmc5883l.h"
#include "bh1750.h"
#include "usart.h"

#define MMA_ADR 0x3c

u32 I2CStep;
s16  Buffer1[3];
u8 hmc5883l_buf[2];


void HMC_Delay(void)
{
	I2CStep=10;
	while(I2CStep--);
}


u8 HMC5883_Init(void)
{    
	HMC_GPIO_Config();
	HMC_I2C_Write(0x02,0x00);//
//	HMC_I2C_Write(0x01,0xe0);
   return HMC_I2C_Read(10);   
}


void HMC_GPIO_Config(void) 
{
 //    IIC_Init();  //IIC总线初始化
} 


u8 HMC_Start(void)
{
	HMC_SDA_H;
	HMC_Delay();
	HMC_SCL_H;
	HMC_Delay();
	if(!HMC_SDA) return 0;//
	HMC_SDA_L;
	return 1;
}


void HMC_I2C_Stop(void)
{
	HMC_SCL_L;
	HMC_Delay();
	HMC_SDA_L;
	HMC_Delay();
	HMC_SCL_H;
	HMC_Delay();
	HMC_SDA_H;
	HMC_Delay();
}


void HMC_I2C_Ack(void)
{
	HMC_SCL_L;
	HMC_Delay();
	HMC_SDA_L;
	HMC_Delay();
	HMC_SCL_H;
	HMC_Delay();
	HMC_SCL_L;
	HMC_Delay();
}


void HMC_I2C_NAck(void)
{
	HMC_SCL_L;
    HMC_Delay();
    HMC_SDA_H;
    HMC_Delay();
    HMC_SCL_H;
    HMC_Delay();
    HMC_SCL_L;
    HMC_Delay();
}


u8 HMC_I2C_WaitAck(void)
{
	HMC_SCL_L;
    HMC_Delay();
    HMC_SDA_H;			
    HMC_Delay();
    HMC_SCL_H;
    HMC_Delay();
	//HMC5883_SDA_IN();
    if(HMC_SDA)
    {
		HMC_SCL_L;
		return 0;
    }
    HMC_SCL_L;
    return 1;
}


void HMC_I2C_SendByte(u8 SendByte) //
{
    u8 i=8;
    while(i--)
	{
		HMC_SCL_L;
		HMC_Delay();
		if(SendByte&0x80)
			HMC_SDA_H;
		else
			HMC_SDA_L;
		SendByte<<=1;
		HMC_Delay();
		HMC_SCL_H;
		HMC_Delay();
	}
	HMC_SCL_L;
}


u8 HMC_I2C_ReceiveByte(void)  //
{ 
	u8 i=8;
	u8 ReceiveByte=0;
	//HMC5883_SDA_IN();
	HMC_SDA_H;				
	while(i--)
	{
		ReceiveByte<<=1;      
		HMC_SCL_L;
		HMC_Delay();
		HMC_SCL_H;
		HMC_Delay();	
		if(HMC_SDA)
		{
			ReceiveByte|=0x01;
		}
	}
	HMC_SCL_L;
	return ReceiveByte;
}


u8 HMC_I2C_Write(u8 address,u8 data)
{
	HMC_Start();
	HMC_I2C_SendByte(MMA_ADR);//
	HMC_I2C_Ack();
	HMC_I2C_SendByte(address);   //   
	HMC_I2C_Ack();	
	HMC_I2C_SendByte(data);
	HMC_I2C_Ack();   
	HMC_I2C_Stop(); 

	return 1;	
}


u8 HMC_I2C_Read(u8 address)//
{
	u8 temp=0;
	HMC_Start();
	HMC_I2C_SendByte(MMA_ADR);//
	HMC_I2C_Ack();
	HMC_I2C_SendByte(address);   //     
	HMC_I2C_Ack();
	HMC_Start();
	HMC_I2C_SendByte(MMA_ADR|0x01);//
	HMC_I2C_Ack();
	temp=HMC_I2C_ReceiveByte();
	HMC_I2C_Ack();
	HMC_I2C_Stop();
	return temp;
}


void HMC_MultipleRead(u8* pBuffer)
{
	u8 i=0;
	HMC_Start();
	HMC_I2C_SendByte(0x3c);
	HMC_I2C_WaitAck();
	HMC_I2C_SendByte(0x03);
	HMC_I2C_WaitAck();
	HMC_Start();
	HMC_I2C_SendByte(0x3c+1);
	HMC_I2C_WaitAck();
	for(i=0;i<6;i++)
	{
		pBuffer[i]=HMC_I2C_ReceiveByte();
		if(i == 5)
			HMC_I2C_NAck();
		else
			HMC_I2C_Ack();
	}
	
	HMC_I2C_Stop();
}


void HMC_GetValue(s16* pBuffer)
{
	u8 tmp[6];
	HMC_MultipleRead(tmp);
	pBuffer[0]=(s16) ( (((u16)tmp[0])<<8) + tmp[1] );
	pBuffer[1]=(s16) ( (((u16)tmp[2])<<8) + tmp[3] );
	pBuffer[2]=(s16) ( (((u16)tmp[4])<<8) + tmp[5] );
}       


double HMC5883_GetAngle(void)
{
	s16 x,y,z; 
	double angle; 
	
	HMC_GetValue(Buffer1); 	
	//---------显示X轴     
	x = Buffer1[0]; //Combine MSB and LSB of X Data output register
	z = Buffer1[1]; //Combine MSB and LSB of Z Data output register
	y = Buffer1[2]; //Combine MSB and LSB of Y Data output register
	angle = atan2((double)y,(double)x) * (180 / 3.14159265) + 180; // angle in degrees  // 垂直面

	angle*=10;
	
	return angle;
}

void Hmc5883_Test( void)
{
     u16 Angle_data;
	   u8  Angle_string[] ={"Angle is :     C\n"};
     u8 i;

	   Angle_data = HMC5883_GetAngle() ;

      for(i=0;i<4;i++)
      {
           Angle_string[13-i] = Angle_data%10 +0x30;
				   Angle_data /=10;
      }
			
			Angle_string[14] = Angle_string[13];
			Angle_string[13] = '.';
     

     U1Send_String( Angle_string );    
	
	

}

