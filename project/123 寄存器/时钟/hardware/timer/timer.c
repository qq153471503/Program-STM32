#include "timer.h"
#include"lcd1602.h"

extern char shi,fen,miao,nian1,nian2,yue,ri;

/*******************************************************************************
* 函 数 名         : TIM3_IRQHandler()
* 函数功能		   : 定时器计算时间产生中断
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		miao++;
		LcdWriteSfm(10,miao);		   //将 秒 实时数据显示到液晶屏
		if(miao >= 59)
		{
			miao = -1;
			fen++;
			LcdWriteSfm(7,fen);
			if(fen >= 59)
			{
				fen = -1;
				shi++;
				LcdWriteSfm(4,shi);
				if(shi >= 23)
				{
					shi = -1;
					ri++;
					LcdWriteNyr(11,ri);
					if(ri >= 30)
					{
						ri = -1;
						yue++;
						LcdWriteNyr(8,yue);
						if(yue >= 11)
						{
							yue = -1;
							nian1++;
							LcdWriteNyr(5,nian1);
							if(nian1 >= 98)
							{
								nian1 = -1;
								nian2++;
								LcdWriteNyr(3,nian2);
								if(nian2 >= 98)
								{
									nian2 = -1;
								}
							}
						}
					}
				}
			}
		}
				    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
/*******************************************************************************
* 函 数 名         : TimerInit
* 函数功能		   : 定时器设置并且初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TimerInit(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;//TIM3时钟使能    
 	TIM3->ARR=arr;  //设定计数器自动重装值//刚好1ms    
	TIM3->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->DIER|=1<<6;   //允许触发中断
	TIM3->CR1=0x8000;   //ARPE使能	   
	TIM3->CR1|=0x01;    //使能定时器3
  	MY_NVIC_Init(1,3,TIM3_IRQChannel,2);//抢占1，子优先级3，组2									 
}
