/**********************************************************************************/
//本程序值适用于AT24C02 其他型号需要改动地址
//按下KEY0 将数据写入 AT24C02
//按下KEY1 读取AT24C02中数据 并由串口1打印到PC端
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LedInit();
	KeyInit();
	BeepInit();
	AT24CXX_Init();
	
	while(AT24CXX_Check())
	{
		printf("24c02错误，请检查啊问题所在！\n");
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

