#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"

//UCOSIII���������ȼ��û�������ʹ�ã�
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

int main(void)
{
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
}
