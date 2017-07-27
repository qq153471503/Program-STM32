#include "timer.h"
#include "usart.h"
#include "delay.h"

/*最大检测高电平持续时间：4194303us*/
/*计算方法：计满值0x3f*计数器最大值65536*/

extern u8 TIM5CH1_CAPTURE_STATE;
extern u16 TIM5CH1_CAPTURE_VALUE;

int main(void)
{
	u32 temp = 0;
	
	SystemInit();
	delay_init();
	uart_init(115200);
	Init(0xffff,71);/*ARR寄存器是16位的，所以最大为0xffff 把他装满*/
	
	while(1)
	{
		delay_ms(10);
		
		if(TIM5CH1_CAPTURE_STATE&0x80) 
		{
			temp = TIM5CH1_CAPTURE_STATE&0x3f;
			temp = temp*65536;
			temp = temp+TIM5CH1_CAPTURE_VALUE;
			printf("High:  %d\n",temp);
			TIM5CH1_CAPTURE_STATE = 0;
		}
	}
}

