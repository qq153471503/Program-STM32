#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"

//UCOSIII中以下优先级用户程序不能使用，
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

#define START_TASK_PRIO 3
#define START_STK_SIZE	128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

#define TASK1_TASK_PRIO 3
#define TASK1_STK_SIZE	128
OS_TCB Task1TaskTCB;
CPU_STK TASK1_TASK_STK[START_STK_SIZE];
void task1_task(void *p_arg);

OS_TMR timer1;
OS_TMR timer2;
void timer1_callback(void *P_tmr,void *p_arg);
void timer2_callback(void *P_tmr,void *p_arg);

int lcd_color[14]={	WHITE, RED,   BLUE,  BRED,      
						GRED,  GBLUE, BLACK,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };

						
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	LED_Init();
	LCD_Init();
	BEEP_Init();
	KEY_Init();
	
	LCD_DrawLine(0,108,239,108);		
	LCD_DrawLine(119,108,119,319);		
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,115,314); 		
	LCD_DrawLine(5,130,115,130);		
	
	LCD_DrawRectangle(125,110,234,314); 	
	LCD_DrawLine(125,130,234,130);		
	POINT_COLOR = BLUE;
	LCD_ShowString(6,111,110,16,16,	 "TIMER1:000");
	LCD_ShowString(126,111,110,16,16,"TIMER2:000");
	
	OSInit(&err);
	OS_CRITICAL_ENTER();
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		
				 (CPU_CHAR	* )"start_task", 		
                 (OS_TASK_PTR )start_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )START_TASK_PRIO,     
                 (CPU_STK   * )&START_TASK_STK[0],	
                 (CPU_STK_SIZE)START_STK_SIZE/10,	
                 (CPU_STK_SIZE)START_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				
	OS_CRITICAL_EXIT();		 
	OSStart(&err);      
}
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif	
	
	//创建定时器1
	OSTmrCreate((OS_TMR		*)&timer1,		//定时器1
                (CPU_CHAR	*)"timer1",		//定时器名字
                (OS_TICK	 )20,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)timer1_callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码
	//创建定时器2
	OSTmrCreate((OS_TMR		*)&timer2,		
                (CPU_CHAR	*)"timer2",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//单次定时器
                (OS_TMR_CALLBACK_PTR)timer2_callback,	//定时器2回调函数
                (void	    *)0,			
                (OS_ERR	    *)&err);
				
	OS_CRITICAL_ENTER();
	//创建TASK1任务
	OSTaskCreate((OS_TCB 	* )&Task1TaskTCB,		
				 (CPU_CHAR	* )"task1_task", 		
                 (OS_TASK_PTR )task1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK   * )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				 
	OS_CRITICAL_EXIT();	//退出临界区
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身			
}
void task1_task(void *p_arg)
{
	u8 key,num;
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		key = KEY_Scan(0);
		
		switch(key)
		{
			case WKUP_PRES:
				OSTmrStart(&timer1,&err);
				printf("开启了定时器1！！\n");break;
			case KEY0_PRES:
				OSTmrStart(&timer2,&err);
				printf("开启了定时器2！！\n");break;
			case KEY1_PRES:
				OSTmrStop(&timer1,OS_OPT_TMR_NONE,0,&err);
				OSTmrStop(&timer2,OS_OPT_TMR_NONE,0,&err);
				printf("开启了定时器1！！\n");break;
		}
		num++;
		
		if(num>=50)
		{
			num = 0;
			LED0 = !LED0;
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
	}
}
void timer1_callback(void *P_tmr,void *p_arg)
{
	static u8 count = 0;
	LCD_ShowxNum(62,111,count,3,16,0x80); //显示定时器1的执行次数
	LCD_Fill(6,131,114,313,lcd_color[count%14]);//填充区域
	count++;	
}
void timer2_callback(void *P_tmr,void *p_arg)
{
	static u8 count = 0;
	count++;
	LCD_ShowxNum(182,111,count,3,16,0x80);  //显示定时器1执行次数
	LCD_Fill(126,131,233,313,lcd_color[count%14]); //填充区域
	LED1 = ~LED1;
	printf("定时器2运行结束\r\n");	
}

