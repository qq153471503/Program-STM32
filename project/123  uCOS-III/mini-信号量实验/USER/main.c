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

#define TASK2_TASK_PRIO	5
#define TASK2_STK_SIZE	128
OS_TCB	Task2TaskTCB;
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
void task2_task(void *p_arg);

u8 SemBuf[30];

OS_SEM MySem;

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
	LCD_ShowString(30,50,"TASK1:BLACK");
	LCD_ShowString(30,70,"TASK2:YELLOW");
	LCD_ShowString(30,90,"Print data to serial port");
	
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
	OSSemCreate ((OS_SEM*	)&MySem,
                 (CPU_CHAR*	)"MySem",
                 (OS_SEM_CTR)1,		
                 (OS_ERR*	)&err);
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
	//创建TASK1任务
	OSTaskCreate((OS_TCB 	* )&Task2TaskTCB,		
				 (CPU_CHAR	* )"task2_task", 		
                 (OS_TASK_PTR )task2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK2_TASK_PRIO,     
                 (CPU_STK   * )&TASK2_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//退出临界区
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}
//任务1的任务函数
void task1_task(void *p_arg)
{
	OS_ERR err;
	u8 task1_str[]="Task1 Running!";
	while(1)
	{
		printf("\r\n任务1:\r\n");
		LCD_Fill(0,110,239,319,BLACK);
		OSSemPend(&MySem,0,OS_OPT_PEND_BLOCKING,0,&err); 	//请求信号量
		memcpy(SemBuf,task1_str,sizeof(task1_str)); //向共享资源区拷贝数据
		delay_ms(300);
		printf("%s\r\n",SemBuf);	//串口输出共享资源区数据	
		OSSemPost (&MySem,OS_OPT_POST_1,&err);				//发送信号量
		LED0 = ~LED0;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);   //延时1s
	}
}
//任务1的任务函数
void task2_task(void *p_arg)
{
	OS_ERR err;
	u8 task2_str[]="Task2 Running!";
	while(1)
	{
		printf("\r\n任务2:\r\n");
		LCD_Fill(0,110,239,319,YELLOW);
		OSSemPend(&MySem,0,OS_OPT_PEND_BLOCKING,0,&err); 	//请求信号量
		memcpy(SemBuf,task2_str,sizeof(task2_str)); //向共享资源区拷贝数据
		delay_ms(300);
		printf("%s\r\n",SemBuf);	//串口输出共享资源区数据	
		OSSemPost (&MySem,OS_OPT_POST_1,&err);				//发送信号量
		LED1 = ~LED1;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);   //延时1s
	}
}
