#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "adc.h"
//#include "pic.h" /*图片编码 若显示图片调用此头文件*/
 
int main(void)
{	 
 	u16 temp;
	float zhuanhuan;
	
	u8 lcd_id[12];			//存放LCD ID字符串
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	AdcInit();
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。				 	

	LCD_ShowString(50,50,64,16,16,"ADC-TEST");
	LCD_ShowString(50,70,80,16,16,"ADC Value:");
	LCD_ShowString(50,90,200,16,16,"Valtage:       V");
	
	while(1)
	{
		temp = GetAdcAverage(0,10);
	
		LCD_ShowNum(130,70,temp,4,16);
		
		zhuanhuan = (float)temp*(3.3/4096);
		temp = zhuanhuan;						//自动过滤掉小数位
		LCD_ShowxNum(130,90,temp,1,16,0);
		LCD_ShowChar(139,90,'.',16,0);
		
		zhuanhuan = (zhuanhuan-temp)*1000;
		
		LCD_ShowxNum(143,90,zhuanhuan,3,16,0x80);
		delay_ms(200);
	}
}
