#include "iic.h"
#include "delay.h"

void IIC_Start(void)
{
	SET_SDA_OUT();
	SCL = 1;
	SDA = 1;
	delay_us(4);
	SDA = 0;
	delay_us(4);
	SCL = 0;
}
void IIC_Stop(void)
{
	SET_SDA_OUT();
	SCL = 0;
	SDA = 0;
	delay_us(4);
	SCL = 1;
	SDA = 1;
	delay_us(4);
}
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStrcuture;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStrcuture.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStrcuture.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStrcuture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStrcuture);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_10);
}
u8 IIC_Wait_Ack(void)
{
	u8 ErrorTime = 0;
	
	SET_SDA_IN();
	SDA = 1;
	delay_us(1);
	SCL = 1;
	delay_us(1);
	
	while(SDA_IN)
	{
		ErrorTime++;
		if(ErrorTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL = 0;
	return 0;
}
void IIC_Ack(void)
{
	SCL = 0;
	SET_SDA_OUT();
	SDA = 0;
	delay_us(2);
	SCL = 1;
	delay_us(2);
	SCL = 0;
}
void IIC_NAck(void)
{
	SCL = 0;
	SET_SDA_OUT();
	SDA = 1;
	delay_us(2);
	SCL = 1;
	delay_us(2);
	SCL = 0;
}
void IIC_Send_Byte(u8 txd)
{
	u8 i;
	SET_SDA_OUT();
	SCL = 0;
	for(i=0; i<8; i++)
	{
		SDA = (0x80&txd)>>7;
		txd<<=1;
		delay_us(4);
		SCL = 1;
		delay_us(4);
		SCL = 0;
		delay_us(4);
	}
}
u8 IIC_Read_Byte(u8 ack)
{
	u8 i;
	u8 receive = 0;
	SET_SDA_IN();
	for(i=0; i<8; i++)
	{
		SCL = 0;
		delay_us(4);
		SCL = 1;
		receive<<=1;
		if(SDA_IN)
			receive++;
		delay_us(4);
	}
	
	if(!ack)
		IIC_NAck();
	else
		IIC_Ack();
	return receive;
}

