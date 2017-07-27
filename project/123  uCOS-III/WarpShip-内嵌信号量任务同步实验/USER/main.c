#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "key.h"
#include "led.h"
#include "lcd.h"

//UCOSIII中以下优先级用户程序不能使用，
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

//LCD刷屏时使用的颜色
int lcd_discolor[14]={	WHITE, BLACK, BLUE,  BRED,      
						GRED,  GBLUE, RED,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };

//开始任务
OS_TCB StartTaskTCB;
#define START_TASK_PRIO 3
#define START_STK_SIZE 128
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

//任务1
OS_TCB Task1TaskTCB;
#define TASK1_TASK_PRIO 4
#define TASK1_STK_SIZE 128
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

//任务2
OS_TCB Task2TaskTCB;
#define TASK2_TASK_PRIO 5
#define TASK2_STK_SIZE 128
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
void task2_task(void *p_arg);

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	LED_Init();
	LCD_Init();
	KEY_Init();
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,234,314);	
	LCD_DrawLine(5,130,234,130);
	POINT_COLOR = RED;
	LCD_ShowString(30,111,200,16,16,"Task_Sem Value:  0");
	POINT_COLOR = BLUE;
	
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
                 (OS_ERR 	* )&err);	
//	OSTaskSemSet();
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
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	
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
                 (OS_ERR 	* )&err);	
	//创建任务2
	OSTaskCreate((OS_TCB 	* )&Task2TaskTCB,		//任务控制块
				 (CPU_CHAR	* )"task2_task", 		//任务名字
                 (OS_TASK_PTR )task2_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )TASK2_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&TASK2_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//退出临界区
	OSTaskSuspend((OS_TCB *)0,&err);/*挂起任务自身*/
}
void task1_task(void *p_arg)
{
	OS_ERR err;
	u8 key;
	u8 num;
	
	while(1)
	{
		key = KEY_Scan(0);
		if(key == WKUP_PRES)
		{
			OSTaskSemPost(&Task2TaskTCB,OS_OPT_POST_NONE,&err);	
			LCD_ShowxNum(150,111,Task2TaskTCB.SemCtr,3,16,0);		//显示信号量值
		}
		num++;
		if(num == 50)
		{
			num = 0;
			LED0 = !LED0;
		}
		delay_ms(10);
	}
}
void task2_task(void *p_arg)
{
	u8 num;
	OS_ERR err;
	
	while(1)
	{
		num++;
		OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
		LCD_ShowxNum(150,111,Task2TaskTCB.SemCtr,3,16,0);	//显示任务内建信号量值
		LCD_Fill(6,131,233,313,lcd_discolor[num%14]);		//刷屏
		LED1 = ~LED1;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   //延时1s
	}
}
