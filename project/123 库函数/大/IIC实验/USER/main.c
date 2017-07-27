/**********************************************************************************/
//������ֵ������AT24C02 �����ͺ���Ҫ�Ķ���ַ
//����KEY0 ������д�� AT24C02
//����KEY1 ��ȡAT24C02������ ���ɴ���1��ӡ��PC��
/**********************************************************************************/

#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "iic.h"
#include "at24cxx.h"

const u8 table[] = "STM32 IIC TEST!";

#define SIZE sizeof(table)
	
int main(void)
{
	u8 ReadData[SIZE];
	SystemInit();
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LedInit();
	KeyInit();
	BeepInit();
	AT24CXX_Init();
	
	while(AT24CXX_Check())
	{
		printf("24c02�������鰡�������ڣ�\n");
		delay_ms(1000);
		led0 = !led0;
	}
	
	while(1)
	{
		if(key0 == 0)
		{
			AT24CXX_Write(0,(u8 * )table,SIZE);
			printf("Write Finished! SIZE:%d\n",SIZE);
		}while(!key0);
		
		if(key1 == 0)
		{
			AT24CXX_Read(0,ReadData,SIZE);
			printf("DATA:%s\n",ReadData);
		}while(!key1);
	}
}

