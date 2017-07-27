#include <string.h>
#include "sys.h"
#include "usart.h"
#include "test.h"
#include "led.h"
//V1.3
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//u8 buff[64],T;
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误 
u8 USART1_RX_BUF[8];     //接收缓冲,最大8个字节.  	
u8 USART2_RX_BUF[8];     //接收缓冲,最大8个字节.
//接收状态
//bit7:接收是否完成标志
u8 flag1=0,flag2=0;
u8 USART_RX_STA=0;       // 接收状态标记	 
u8 RX_num1=0,RX_num2=0;     // 接收到的有效字节数目
void USART1_IRQHandler(void)
{			
	u8 res,sum;
	res=USART1->DR;
	
	if(RX_num1>0)
	{
	   USART1_RX_BUF[RX_num1]=res;
	   RX_num1++;
	}
	else if (res==0x55)		// 寻找包头
	{
		
	   USART1_RX_BUF[0]=res;
	   RX_num1=1;
	}
	
	if(RX_num1>=8)
	{
		RX_num1=0;
		if(USART1_RX_BUF[7]==0xbb)	 // 判断包尾
		{									  
		    //主指令与三位副指令左求和校验
			//注意：在求和溢出时应该对和做256取余。
			 sum=(USART1_RX_BUF[2]+USART1_RX_BUF[3]+USART1_RX_BUF[4]+USART1_RX_BUF[5])%256;
			if(sum==USART1_RX_BUF[6])
			{
					USART_RX_STA|=0x80;
					flag1=1;	//   指令验证正确，标志位置1

					LED2=0;		//关闭蜂鸣器
			}
			else {LED2=1; flag1=0;}
		}

		
		else {LED2=1; flag1=0;}		 //接收错误指令，打开蜂鸣器
	}
		 								 
}  
void USART2_IRQHandler(void)
{
    u8 res,sum;
	res=USART2->DR;
	
	if(RX_num2>0)
	{
	   USART2_RX_BUF[RX_num2]=res;
	   RX_num2++;
	}
	else if (res==0x55)		// 寻找包头
	{
		
	   USART2_RX_BUF[0]=res;
	   RX_num2=1;
	}
	
	if(RX_num2>=8)
	{
		RX_num2=0;
		if(USART2_RX_BUF[7]==0xbb)	 // 判断包尾
		{									  
		    //主指令与三位副指令左求和校验
			//注意：在求和溢出时应该对和做256取余。
			 sum=(USART2_RX_BUF[2]+USART2_RX_BUF[3]+USART2_RX_BUF[4]+USART2_RX_BUF[5])%256;
			if(sum==USART2_RX_BUF[6])
			{
					USART_RX_STA|=0x80;
					flag2=1;	//   指令验证正确，标志位置1

					LED3=0;		//关闭蜂鸣器
			}
			else {LED3=1; flag2=0;}
		}

		
		else {LED3=1; flag2=0;}		 //接收错误指令，打开蜂鸣器
	}					 
											 
} 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH=0X444444B4;//IO状态设置
		  
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#ifdef EN_USART1_RX		  //如果使能了接收
	//使能接收中断
	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,1,USART1_IRQChannel,1);//组2，最低优先级 
#endif
}										 
//初始化IO 串口2
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*32);//得到USARTDIV  串口1时(bound*16)
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB2ENR|=1<<0;   //使能AFIO口时钟
	RCC->APB2ENR|=1<<5;   //使能PORT5口时钟  
	RCC->APB1ENR|=1<<17;  //使能串口2时钟
	
	AFIO->MAPR=0X0008; 	  // 串口2重映射功能TX-PD5  RX-PD6
	GPIOD->CRL=0X44B44444;//IO状态设置
		  
	RCC->APB1RSTR|=1<<17;   //复位串口2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.
#ifdef EN_USART2_RX		  //如果使能了接收
	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,2,USART2_IRQChannel,1);//组2，最低优先级 
#endif
}

void Delay(vu32 nCount) 
{
  for(; nCount != 0; nCount--);
}
int U1SendChar(int ch) 
{
  while (!(USART1->SR & USART_FLAG_TXE));
  USART1->DR = (ch & 0x1FF);
  return (ch);
}
//发送一个字符
int U2SendChar(int ch) 
{

  while (!(USART2->SR & USART_FLAG_TXE));
  USART2->DR = (ch & 0x1FF);

  return (ch);
}

int GetKey(void) 
{
  while (!(USART2->SR & USART_FLAG_RXNE));  

  return ((int)(USART2->DR & 0x1FF));
}
