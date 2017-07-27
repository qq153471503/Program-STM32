#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "lcd.h"
#include "led.h"
#include "key.h"

//UCOSIII中以下优先级用户程序不能使用，
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

#define START_TASK_PRIO 3
#define START_STK_SIZE 128
OS_TCB START_TASK_TCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

#define TASK1_TASK_PRIO 4
#define TASK1_STK_SIZE 128
OS_TCB TASK1_TASK_TCB;
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

#define TASK2_TASK_PRIO 5
#define TASK2_STK_SIZE 128
OS_TCB TASK2_TASK_TCB;
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
void task2_task(void *p_arg);

#define KYE0_FLAG 0X01
#define KEY1_FLAG 0X02
#define KEYFLAG_VALUE 0X00

//定义一个事件标志组
OS_FLAG_GRP EVENTFLAGS;

//LCD刷屏时使用的颜色
int lcd_discolor[14]={	WHITE, BLACK, BLUE,  BRED,      
						GRED,  GBLUE, RED,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };


int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	KEY_Init();
	LED_Init();
	LCD_Init();

	
	OSInit(&err);		    //初始化UCOSIII
	OS_CRITICAL_ENTER();	//进入临界区			 
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&START_TASK_TCB,		//任务控制块
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
		
	OS_CRITICAL_ENTER();	//进入临界区
	//创建一个事件标志组
	OSFlagCreate((OS_FLAG_GRP*)&EVENTFLAGS,		//指向事件标志组
                 (CPU_CHAR*	  )"EVENTFLAGS",	//名字
                 (OS_FLAGS	  )KEYFLAG_VALUE,	//事件标志组初始值
                 (OS_ERR*  	  )&err);			//错误码
	//创建任务1
	OSTaskCreate((OS_TCB*     )&TASK1_TASK_TCB,		
				 (CPU_CHAR*   )"task1_task", 		
                 (OS_TASK_PTR )task1_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK*    )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//创建任务2
	OSTaskCreate((OS_TCB*     )&TASK2_TASK_TCB,		
				 (CPU_CHAR*   )"task2_task", 		
                 (OS_TASK_PTR )task2_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TASK2_TASK_PRIO,     
                 (CPU_STK*    )&TASK2_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	OS_CRITICAL_EXIT();	//退出临界区
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}

void task1_task(void *p_arg)
{
	OS_ERR err;
	u8 num;
	OS_FLAGS flags_num;
	
	while(1)
	{	
		if(KEY0 == 0)
		{
			//向事件标志组EventFlags发送标志
			flags_num=OSFlagPost((OS_FLAG_GRP*)&EVENTFLAGS,
								 (OS_FLAGS	  )KYE0_FLAG,
								 (OS_OPT	  )OS_OPT_POST_FLAG_SET,
					             (OS_ERR*	  )&err);
			LCD_ShowNum(174,110,flags_num,1,16);
			printf("事件标志组EventFlags的值:%d\r\n",flags_num);
			while(!KEY0);
		}else if(KEY1 == 0)
		{
			//向事件标志组EventFlags发送标志
			flags_num=OSFlagPost((OS_FLAG_GRP*)&EVENTFLAGS,
								 (OS_FLAGS	  )KEY1_FLAG,
								 (OS_OPT	  )OS_OPT_POST_FLAG_SET,
								 (OS_ERR*     )&err);
			LCD_ShowNum(174,110,flags_num,1,16);
			printf("事件标志组EventFlags的值:%d\r\n",flags_num);
			while(!KEY1);
		}
		num++;
		if(num == 50)
		{
			num =0;
			LED0 = !LED0;
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //延时10ms
	}
}
void task2_task(void *p_arg)
{
	u8 num;
	OS_ERR err;
	while(1)
	{
		//等待事件标志组
		OSFlagPend((OS_FLAG_GRP*)&EVENTFLAGS,
				   (OS_FLAGS	)KYE0_FLAG+KEY1_FLAG,
		     	   (OS_TICK     )0,
				   (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING,
				   (CPU_TS*     )0,
				   (OS_ERR*	    )&err);
		num++;
		LED1=!LED1;
		LCD_Fill(6,131,233,313,lcd_discolor[num%14]);
		LCD_ShowNum(174,130 ,EVENTFLAGS.Flags,1,16);
		printf("事件标志组EventFlags的值:%d\r\n",EVENTFLAGS.Flags);
		LCD_ShowNum(174,110,EVENTFLAGS.Flags,1,16);
	}
}

