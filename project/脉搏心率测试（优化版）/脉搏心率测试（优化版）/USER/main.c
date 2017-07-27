#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"
#include "stdio.h"
#include "PWM.h"

extern int IBI;          //相邻节拍时间
extern int BPM;          //心率值             
extern int Signal;       //原始信号值            
extern unsigned char QS; //发现心跳标志

//上位机发送函数
void sendDataToProcessing(u8 symbol, int16_t dat )
{
    putchar(symbol);       // symbol prefix tells Processing what type of data is coming
		printf("%d\r\n",dat);	 // the data to send culminating in a carriage return
}

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	delay_init();
	LED_Init();
	NVIC_Configuration();
	uart_init(115200);
	LCD_Init();
	Adc_Init();
	TIM3_Int_Init(1999,71);//定时周期2ms
	LED1=0;

  POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(60,130,200,16,16,"* Signal:");	      
	LCD_ShowString(60,150,200,16,16,"* BPM: ");	
	LCD_ShowString(60,170,200,16,16,"* IBI");	  
	
	/* TIM3 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	
	while (1)
  {
		//sendDataToProcessing('S', Signal); // send Processing the raw Pulse Sensor data	
    LCD_ShowxNum(156,130,Signal,4,16,0);		
		if (QS == true)
		{
			//sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
			//printf("****B %d \r\n",BPM);
			LCD_ShowxNum(156,150,BPM,4,16,0);
				
			//sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
			//printf("****Q %d \r\n",IBI);			
			LCD_ShowxNum(156,170,IBI,4,16,0);
			
		  QS = false; // reset the Quantified Self flag for next time
				
			//对BPM值进行范围划分
			if(BPM>60&&BPM<100)
				LCD_ShowString(60,210,200,16,16,"Good!");
			else	
				LCD_ShowString(60,210,200,16,16,"Bad!");	
		}		
		delay_ms(20);	
	}
}


