/*������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
//ʵ�����ƣ�ս�������� SPI FLASH(W25Q64) ʵ��										  */
//																					  */
//�����飺ͨ��SPIЭ�� ����key0��W25Q64д��һ���ַ���								  */
//			����key1��ȡ���ݣ�������ȡ��������ͨ������1���͵�PC��					  */
//																					  */
//��ʵ��SPI��������ȫ˫��ģʽ�������ڴ��ڻ���ʾ���顣								  */
/*������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "spi.h"
#include "flash.h"

const u8 table[] = "SPI FLASH TEST!";
#define SIZE sizeof(table)		/*�������鳤��*/
u8 ReadData[SIZE];
int main(void)
{
	u8 i;
	SystemInit();
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LedInit();
	BeepInit();
	KeyInit();
	SPI_Flash_Init(); 
	
	while(SPI_Flash_ReadID() != W25Q64)		/*����ⲿflash�ͺ�*/
	{
		led0 = !led0;				/*led����˸     */
		beep = !beep;				/*����������    ����*/
		delay_ms(300);
		printf("\nFLASH�������飡");
	}
	
	while(1)
	{
		if(key0 == 0)
		{
			SPI_Flash_Write((u8 *)table,0,SIZE);
			printf("\nд��FLASH��ɣ�");
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
			led1 = !led1;/*��ɫled*/
		}
	}

}

