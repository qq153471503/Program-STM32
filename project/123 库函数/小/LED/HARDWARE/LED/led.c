#include"led.h"

void LedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);//�����˿�x��ʱ��  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;     //����I/O�˿�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //��������/���ģʽ�ķ�ʽ�ٶ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������/���ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //ִ����������
	GPIO_SetBits(GPIOA,GPIO_Pin_8);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;     //����I/O�˿�
	GPIO_Init(GPIOD, &GPIO_InitStructure);    //ִ����������
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
}


