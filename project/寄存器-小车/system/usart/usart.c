#include <string.h>
#include "sys.h"
#include "usart.h"
#include "test.h"
#include "led.h"
//V1.3
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//u8 buff[64],T;
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 
u8 USART1_RX_BUF[8];     //���ջ���,���8���ֽ�.  	
u8 USART2_RX_BUF[8];     //���ջ���,���8���ֽ�.
//����״̬
//bit7:�����Ƿ���ɱ�־
u8 flag1=0,flag2=0;
u8 USART_RX_STA=0;       // ����״̬���	 
u8 RX_num1=0,RX_num2=0;     // ���յ�����Ч�ֽ���Ŀ
void USART1_IRQHandler(void)
{			
	u8 res,sum;
	res=USART1->DR;
	
	if(RX_num1>0)
	{
	   USART1_RX_BUF[RX_num1]=res;
	   RX_num1++;
	}
	else if (res==0x55)		// Ѱ�Ұ�ͷ
	{
		
	   USART1_RX_BUF[0]=res;
	   RX_num1=1;
	}
	
	if(RX_num1>=8)
	{
		RX_num1=0;
		if(USART1_RX_BUF[7]==0xbb)	 // �жϰ�β
		{									  
		    //��ָ������λ��ָ�������У��
			//ע�⣺��������ʱӦ�öԺ���256ȡ�ࡣ
			 sum=(USART1_RX_BUF[2]+USART1_RX_BUF[3]+USART1_RX_BUF[4]+USART1_RX_BUF[5])%256;
			if(sum==USART1_RX_BUF[6])
			{
					USART_RX_STA|=0x80;
					flag1=1;	//   ָ����֤��ȷ����־λ��1

					LED2=0;		//�رշ�����
			}
			else {LED2=1; flag1=0;}
		}

		
		else {LED2=1; flag1=0;}		 //���մ���ָ��򿪷�����
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
	else if (res==0x55)		// Ѱ�Ұ�ͷ
	{
		
	   USART2_RX_BUF[0]=res;
	   RX_num2=1;
	}
	
	if(RX_num2>=8)
	{
		RX_num2=0;
		if(USART2_RX_BUF[7]==0xbb)	 // �жϰ�β
		{									  
		    //��ָ������λ��ָ�������У��
			//ע�⣺��������ʱӦ�öԺ���256ȡ�ࡣ
			 sum=(USART2_RX_BUF[2]+USART2_RX_BUF[3]+USART2_RX_BUF[4]+USART2_RX_BUF[5])%256;
			if(sum==USART2_RX_BUF[6])
			{
					USART_RX_STA|=0x80;
					flag2=1;	//   ָ����֤��ȷ����־λ��1

					LED3=0;		//�رշ�����
			}
			else {LED3=1; flag2=0;}
		}

		
		else {LED3=1; flag2=0;}		 //���մ���ָ��򿪷�����
	}					 
											 
} 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH=0X444444B4;//IO״̬����
		  
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
#ifdef EN_USART1_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж�
	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,1,USART1_IRQChannel,1);//��2��������ȼ� 
#endif
}										 
//��ʼ��IO ����2
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
//CHECK OK
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*32);//�õ�USARTDIV  ����1ʱ(bound*16)
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB2ENR|=1<<0;   //ʹ��AFIO��ʱ��
	RCC->APB2ENR|=1<<5;   //ʹ��PORT5��ʱ��  
	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���2ʱ��
	
	AFIO->MAPR=0X0008; 	  // ����2��ӳ�书��TX-PD5  RX-PD6
	GPIOD->CRL=0X44B44444;//IO״̬����
		  
	RCC->APB1RSTR|=1<<17;   //��λ����2
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.
#ifdef EN_USART2_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,2,USART2_IRQChannel,1);//��2��������ȼ� 
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
//����һ���ַ�
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
