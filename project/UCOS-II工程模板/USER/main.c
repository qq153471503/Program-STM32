#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"

int main(void)
{
	delay_init();       	//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ�������
	uart_init(115200);    	//���ڲ���������

}

