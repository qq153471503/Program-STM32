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

OS_SEM sem1;
OS_SEM sem2;
OS_Q q_queue;
#define QUEUE_NUM 10
#define CORE_NUM 3

//用于存储消息队列的内容
u8 QUEUE_TABLE[] = "1234567890";

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
	LCD_Init();
	LED_Init();
	KEY_Init();

	POINT_COLOR = RED;
	LCD_ShowString(30,10,200,16,16,"ALIENTEK STM32F1");	
	LCD_ShowString(30,30,200,16,16,"UCOSIII Examp 13-1");
	LCD_ShowString(30,50,200,16,16,"Pend Multi");
	LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,16,16,"2015/5/20");
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,234,314);	
	LCD_DrawLine(5,130,234,130);
	POINT_COLOR = RED;
	LCD_ShowString(50,111,200,16,16,"ObjRdy_NUM: 0");
	POINT_COLOR = BLUE;
	
	OSInit(&err);
	OS_CRITICAL_ENTER();
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
	//创建信号量sem1
	OSSemCreate ((OS_SEM*	)&sem1,
                 (CPU_CHAR*	)"sem1",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);
	//创建信号量sem2
	OSSemCreate ((OS_SEM*	)&sem2,
                 (CPU_CHAR*	)"sem2",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);
	//创建消息队列
	OSQCreate ((OS_Q*		)&q_queue,	//消息队列
                (CPU_CHAR*	)"q_queue",	//消息队列名称
                (OS_MSG_QTY	)QUEUE_NUM,	//消息队列长度
                (OS_ERR*	)&err);		//错误码
	//创建TASK1任务
	OSTaskCreate((OS_TCB 	* )&TASK1_TASK_TCB,		
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
	//创建TASK2任务
	OSTaskCreate((OS_TCB 	* )&TASK2_TASK_TCB,		
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
	OSTaskDel((OS_TCB*)&START_TASK_TCB,&err);	//删除start_task任务自身				 
}
void task1_task(void *p_arg)
{
	u8 key;
	u8 num = 0;
	OS_ERR err;
	u8 *data;
	u8 size;
	
	while(1)
	{
		key = KEY_Scan(0);		
		switch(key)
		{
			case KEY0_PRES:
					OSSemPost(&sem1,OS_OPT_POST_1,&err);
					break;
			case KEY1_PRES:
					OSSemPost(&sem2,OS_OPT_POST_1,&err);
					break;
			case KEY2_PRES:
				//发送消息
				OSQPost((OS_Q*		)&q_queue,		
						(void*		)QUEUE_TABLE,
						(OS_MSG_SIZE)sizeof(QUEUE_TABLE),
						(OS_OPT		)OS_OPT_POST_FIFO,
						(OS_ERR*	)&err);break;
			case WKUP_PRES:
				//请求消息KEY_Msg
			data = OSQPend((OS_Q*			)&q_queue,   
							(OS_TICK		)0,
							(OS_OPT			)OS_OPT_PEND_BLOCKING,
							(OS_MSG_SIZE*	)&size,//储存接收到消息的大小（字节）		
							(CPU_TS*		)0,
							(OS_ERR*		)&err);			
			printf("%s\r\n\n\n",data);break;
			/*打印出请求的消息内容*/
		}	
		num++;
		if(num >= 50)
		{
			num = 0;
			LED0 = !LED0;//红led灯
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
void task2_task(void *p_arg)
{
	OS_ERR err;
	OS_OBJ_QTY num;
	u8 count;
	OS_PEND_DATA table[QUEUE_NUM];
	
	table[0].PendObjPtr = (OS_PEND_OBJ *)&sem1;
	table[1].PendObjPtr = (OS_PEND_OBJ *)&sem2;
	table[2].PendObjPtr = (OS_PEND_OBJ *)&q_queue;
	
	while(1)
	{
		num = OSPendMulti((OS_PEND_DATA*	)table,	
						  (OS_OBJ_QTY		)CORE_NUM,	//内核数量
						  (OS_TICK		   	)0,		
						  (OS_OPT         	)OS_OPT_PEND_BLOCKING,
						  (OS_ERR*			)&err);
		LCD_ShowNum(147,111,num,1,16);  					//显示当前有几个内核对象准备好了		
		count++;
		LED1 = !LED1;
		LCD_Fill(6,131,233,313,lcd_discolor[count%14]);		//刷屏
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
	}	
}

