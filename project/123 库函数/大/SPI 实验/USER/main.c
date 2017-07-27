/*————————————————————————————————————————————————————————————————————————————————————*/
//实验名称：战舰开发板 SPI FLASH(W25Q64) 实验										  */
//																					  */
//程序简介：通过SPI协议 按下key0向W25Q64写入一串字符，								  */
//			按下key1读取数据，并将读取到的数据通过串口1发送到PC。					  */
//																					  */
//本实验SPI置于两线全双工模式，所以在串口会显示两遍。								  */
/*————————————————————————————————————————————————————————————————————————————————————*/
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "spi.h"
#include "flash.h"

const u8 table[] = "SPI FLASH TEST!";
#define SIZE sizeof(table)		/*测量数组长度*/
u8 ReadData[SIZE];
int main(void)
{
	u8 i;
	SystemInit();
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LedInit();
	BeepInit();
	KeyInit();
	SPI_Flash_Init(); 
	
	while(SPI_Flash_ReadID() != W25Q64)		/*检测外部flash型号*/
	{
		led0 = !led0;				/*led灯闪烁     */
		beep = !beep;				/*蜂鸣器响灭    报警*/
		delay_ms(300);
		printf("\nFLASH错误，请检查！");
	}
	
	while(1)
	{
		if(key0 == 0)
		{
			SPI_Flash_Write((u8 *)table,0,SIZE);
			printf("\n写入FLASH完成！");
		}while(!key0);
		
		if(key1 == 0)
		{
			SPI_Flash_Read(ReadData,0,SIZE);
			printf("\n%s",ReadData);
		}while(!key1);
		
		i++;
		delay_ms(10);
		if(i == 20)
		{
			i = 0;
			led1 = !led1;/*黄色led*/
		}
	}

}

