#include <string.h>
//#include "stm32_lib_config.h"
#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"
#include "test.h"
#include "led.h"
#include "uart_my.h"
#include "exit.h"
#include "xj.h"
#define  RXD1_MAX_NUM  200

u8 USART1_RX_BUF[RXD1_MAX_NUM];     //接收缓冲,最大200个字节.  	
u8 USART2_RX_BUF[MAX_NUM];     //接收缓冲,最大8个字节.
//接收状态
//bit7:接收是否完成标志
u8 flag1=0,flag2=0,rx2zt_flag=0;   //串口接收状态;
u8 flag3=0;		 //add by ygm



u8 USART_RX_STA=0;       // 接收状态标记	 
u8 RX_num1=0,RX_num2=0;     // 接收到的有效字节数目
u8 RX2_MAX=8;			// 定义接收的最长字节
u8 USART1_RXNUM=0;      // 定义串口1接收有效数据长度
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
u8 USART_RX_BUF[USART_REC_LEN];

void USART1_IRQHandler(void)
{			

   		u8 Res;
	#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
	#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif

//	if(rxd1_timer4_flag==1) //  2ms时间 数据帧重新开始
//	{
//		 rxd1_timer4_flag=0;
//		 TIM4->CR1|=0x01;     //使能定时器4
//		 USART1_RXNUM=0;
//		 USART1_RX_BUF[USART1_RXNUM]=res;

//	}
//	else if(USART1_RXNUM<RXD1_MAX_NUM)	
//	{
//		  USART1_RX_BUF[++USART1_RXNUM]=res;	 // 接收数据帧
//	}

//	else   // 数据超出最大接收数据长度时，舍去不要
//	{
//			res=0;
//	}  

	 								 
}  
 void USART2_IRQHandler(void)
 {
   static u8 res;

 	res= USART2->DR;
//	 gd =res;
	USART2_RX_BUF[RX_num2++] =res;
	 if(RX_num2 >2 ) // RX_num2 =3
	 {

		 RX_num2 =0;
		 if(USART2_RX_BUF[2]  == (USART2_RX_BUF[0] ^ USART2_RX_BUF[1]))
		 {
		    flag2 =1;
			 rx2zt_flag =1;
//			 LED1 =!LED1;
		 }
		 else {
			 
			 flag2 =0;
			 rx2zt_flag =0;
		   USART2_RX_BUF[0] =USART2_RX_BUF[1];
			 USART2_RX_BUF[1] =USART2_RX_BUF[2];
		   RX_num2 =2;
		 }
	 
	 }		 

// 	if(RX_num2>0)
// 	{
// 	   USART2_RX_BUF[RX_num2]=res;
// 	   RX_num2++;
// 	}
// 	else if (res==0x55)		// 寻找包头
// 	{
// 		
// 	   USART2_RX_BUF[0]=res;
// 	   RX_num2=1;
// 	}
// 	
//   if(RX_num2>=RX2_MAX)
// 	{
//  		RX_num2=0;	
// 		if(USART2_RX_BUF[RX2_MAX-1]==0xbb)	 // 判断包尾
// 		{									  
// 		    //主指令与三位副指令左求和校验
// 			//注意：在求和溢出时应该对和做256取余。
// 			RX_num2=0;	  // 计数清零
// 			rx2zt_flag=0;  // 接收状态清零
// 			sum=(USART2_RX_BUF[2]+USART2_RX_BUF[3]+USART2_RX_BUF[4]+USART2_RX_BUF[5])%256;
// 			if(sum==USART2_RX_BUF[6])
// 			{
// 				//	USART_RX_STA|=0x80;
// 					flag2=1;	//   指令验证正确，标志位置1
//          rx2zt_flag =1;
// 					LED2=0;		//关闭蜂鸣器
// 			}
// 			else {LED1=2; flag2=0;rx2zt_flag=0;}
// 		}		
// 		else {LED1=2; flag2=0;rx2zt_flag=0;}		 //接收错误指令，打开蜂鸣器
// 	}
 					  											 
 } 
void uart1_init(u32 bound)
{  	 
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10


   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
#if EN_USART1_RX		  //如果使能了接收  
   //Usart1 NVIC 配置
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(USART1, ENABLE);                    //使能串口 
		USART_GetFlagStatus(USART1,USART_FLAG_TC);
		//USART_ClearFlag(USART1, USART_FLAG_TC);
}										 
//初始化IO 串口2
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
// mode 初始化方式 0 开中断     TXD -> PA2   RXD -> PA3
//                 1 不开中断   TXD -> PD5   RXD -> PD6
void uart2_init(u32 bound ,u8 mode)
{  	 
	if( mode ) //不开中断   TXD -> PD5   RXD -> PD6
	{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2，GPIOD时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOD, ENABLE);

  GPIO_PinRemapConfig(GPIO_Remap_USART2 , ENABLE);
	 //USART2_TX   PD.5
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PD5
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化PD5
   
    //USART2_RX	  PD6
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOD, &GPIO_InitStructure);  //初始化PD6

   //USART 初始化设置
	USART_DeInit(USART2);  //复位串口2
	
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口
		
  USART_Cmd(USART2, ENABLE);                    //使能串口 
 
	}
	else  //开中断     TXD -> PA2   RXD -> PA3
	{
    GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	 
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2，GPIOA时钟
 	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);	
		
		USART_DeInit(USART2);  //复位串口2
	 //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3

   //USART 初始化设置

	  USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	  USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口2
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断

    USART_Cmd(USART2, ENABLE);                    //使能串口 
		USART_GetFlagStatus(USART2,USART_FLAG_TC);	
	
	}
 

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

void U1Send_String( u8 *s)
{
     while ( *s)
		 {
         U1SendChar( *s++);

     }
 }

 
