#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "lcd.h"
#include "led.h"
#include "key.h"

//UCOSIII中以下优先级用户程序不能使用，ALIENTEK
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

#define START_TASK_PRIO	3
#define START_STK_SIZE	128
OS_TCB	StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

#define TASK1_TASK_PRIO	4
#define TASK1_STK_SIZE	128
OS_TCB	Task1TaskTCB;
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

OS_TMR timer0;
OS_TMR timer1;
void timer0_callback(void *p_tmr,void *p_arg);
void timer1_callback(void *p_tmr,void *p_arg);


//LCD刷屏时的颜色
int lcd_color[14]={	WHITE, RED,   BLUE,  BRED,      
						GRED,  GBLUE, BLACK,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };
u8 key;
	
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();
	uart_init(9600);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	LCD_Init();
	KEY_Init();
	
	POINT_COLOR = RED;
	LCD_ShowString(30,10,"WeiHai College");	
	LCD_ShowString(30,30,"UCOSIII");
	LCD_ShowString(30,50,"KEY_UP:Start Tmr1");
	LCD_ShowString(30,70,"KEY0:Start Timer2");
	LCD_ShowString(30,90,"KEY1:Stop Timer1 and Timer2");
	
	LCD_DrawLine(0,108,239,108);		//画线
	LCD_DrawLine(119,108,119,319);		
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,115,314); 	//画一个矩形
	LCD_DrawLine(5,130,115,130);		//画线
	
	LCD_DrawRectangle(125,110,234,314); //画矩形
	LCD_DrawLine(125,130,234,130);		//画线
	POINT_COLOR = BLUE;
	LCD_ShowString(6,111,"TIMER1:000");
	LCD_ShowString(126,111,"TIMER2:000");
	
	OSInit(&err);
	OS_CRITICAL_ENTER();
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start_task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);      //开启UCOSIII
}
//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif	

	//创建定时器1
	OSTmrCreate((OS_TMR		*)&timer0,		//定时器1
                (CPU_CHAR	*)"timer0",		//定时器名字
                (OS_TICK	 )20,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)timer0_callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码
	//创建定时器2
	OSTmrCreate((OS_TMR		*)&timer1,		
                (CPU_CHAR	*)"timer1",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//单次定时器
                (OS_TMR_CALLBACK_PTR)timer1_callback,	//定时器2回调函数
                (void	    *)0,			
                (OS_ERR	    *)&err);

				
	OS_CRITICAL_ENTER();
	//创建任务1
	OSTaskCreate((OS_TCB 	* )&Task1TaskTCB,		//任务控制块
				 (CPU_CHAR	* )"task1_task", 		//任务名字
                 (OS_TASK_PTR )task1_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )TASK1_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&TASK1_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OS_TaskSuspend((OS_TCB *)&StartTaskTCB,&err);
//	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}
void task1_task(void *p_arg)
{
	u8 num;
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		key = KEY_Scan();
		
		switch(key)
		{
			case KEY0_UP: 
				OSTmrStart(&timer0,&err);
				printf("开启定时器1！\r\n");break;
			case KEY1_UP:
				OSTmrStart(&timer1,&err);
				printf("开启了定时器2!\r\n");break;
			case WK_UP:
				OSTmrStop(&timer0,OS_OPT_TMR_NONE,0,&err);
				OSTmrStop(&timer1,OS_OPT_TMR_NONE,0,&err);
				printf("关闭了定时器1和2!!\r\n");break;
		}
		
		num++;
		if(num==50)
		{
			num = 0;
			LED0 = !LED0;
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
	}
}
void timer0_callback(void *p_tmr,void *p_arg)
{
	static u8 timer0_count = 0;
	
	LCD_ShowNum(62,111,timer0_count,3,16); //显示定时器1的执行次数
	LCD_Fill(6,131,114,313,lcd_color[timer0_count%14]);//填充区域
	timer0_count++;
}
void timer1_callback(void *p_tmr,void *p_arg)
{
	static u8 timer1_count = 0;
	timer1_count++;		//定时器2执行次数加1
	LCD_ShowNum(182,111,timer1_count,3,16);  //显示定时器1执行次数
	LCD_Fill(126,131,233,313,lcd_color[timer1_count%14]); //填充区域
	LED1 = ~LED1;
	printf("定时器2运行结束\r\n");
}


