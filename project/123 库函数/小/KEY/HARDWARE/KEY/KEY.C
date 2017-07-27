
		#include"key.h"

void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);//开启端口x的时钟  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;     //设置I/O端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //设置输出/输出模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //执行上面三句

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;     //设置I/O端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //设置输出/输出模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //执行上面三句
}

/*mode = 1  支持连续按键
  mode = 0  不支持连续按键*/
u8 KeyScan(u8 mode)
{
	static u8 key_up = 1;
	
	if(mode) key_up = 1;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag
	if(key_up&&(key0 == 0||key1 == 0||wk_up == 1))
	{
		delay_ms(10);
		key_up = 0;
		if(key0 == 0)
		{
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,DISABLE);	//打开jtag
			return 1;
		}else if(key1 == 0)
		{
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,DISABLE);	//打开jtag
			return 2;
		}else if(wk_up == 1)
		{
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,DISABLE);	//打开jtag
			return 3;
		}
	}else if((key0 == 1)&&(key1 == 1)&&(wk_up == 0))
				key_up = 1;
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,DISABLE);	//打开jtag
			return 0;
}
