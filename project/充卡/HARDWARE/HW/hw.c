#include "stm32f10x.h"	
#include "delay.h"   
#include "hw.h"

/***************************************************************
** 功能：     红外发射端口初始化
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void HW_Init()
{
	RCC->APB2ENR|=1<<6;    //使能PORTE时钟 
	GPIOE->CRL&=0XFFFFFF0F;
	GPIOE->CRL|=0X00000030;	  //PE1推挽输出
	GPIOE->ODR|=0X0002; 	  //PE1输出高
		
	RI_TXD=1; 

}

/***************************************************************
** 功能：     红外发射子程序
** 参数：	  *s：指向要发送的数据
**             n：数据长度
** 返回值：    无
****************************************************************/
void Transmition(u8 *s,int n)
{
    u8 i,j,temp;

    RI_TXD=0;
    delay_ms(9);
    RI_TXD=1;
    delay_ms(4);
	delay_us(560);

	for(i=0;i<n;i++)
	{
		for(j=0;j<8;j++)
		  {
		     temp=(s[i]>>j)&0x01;
		     if(temp==0)//发射0
		        {
		           RI_TXD=0;
		           delay_us(500);//延时0.5ms
		           RI_TXD=1;
		           delay_us(500);//延时0.5ms
		         }
		     if(temp==1)//发射1
		         {
		           RI_TXD=0;
		           delay_us(500);//延时0.5ms
		           RI_TXD=1;
				   delay_ms(1);
		           delay_us(800);//延时1.69ms
		
		         }
		  }
	}
    RI_TXD=0;//结束
    delay_us(560);//延时0.56ms
    RI_TXD=1;//关闭红外发射
}


