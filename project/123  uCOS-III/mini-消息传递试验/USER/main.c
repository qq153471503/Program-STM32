/************************************************************************************************
消息传递实验 
按下KEY1 发送消息
按下wk_up 请求消息 并将请求到的消息打印到串口
************************************************************************************************/

//UCOSIII中以下优先级用户程序不能使用，
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "led.h"
#include "key.h"
#include "beep.h"

//开始任务
#define START_TASK_PRIO 3
#define START_STK_SIZE 128
CPU_STK START_TASK_STK[START_STK_SIZE];
OS_TCB START_TASK_TCB;
void start_task(void *p_arg);

//按键任务
#define KEY_TASK_PRIO 4
#define KEY_STK_SIZE 128
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
OS_TCB KEY_RASK_TCB;
void key_task(void *p_arg);

//创建消息队列
OS_Q KEY_Msg;


u8 data[10] = "1234567890";

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	LED_Init();
	BEEP_Init();
	KEY_Init();
	
	OSInit(&err);
	OS_CRITICAL_ENTER();	//进入临界区	
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&START_TASK_TCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
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
	//创建消息队列KEY_Msg
	OSQCreate ((OS_Q*		)&KEY_Msg,	//消息队列
                (CPU_CHAR*	)"KEY Msg",	//消息队列名称
                (OS_MSG_QTY	)4,	//消息队列长度，这里设置为4
                (OS_ERR*	)&err);		//错误码
	//创建按键任务
	OSTaskCreate((OS_TCB 	* )&KEY_RASK_TCB,		
				 (CPU_CHAR	* )"key_task", 		
                 (OS_TASK_PTR )key_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )KEY_TASK_PRIO,     
                 (CPU_STK   * )&KEY_TASK_STK[0],	
                 (CPU_STK_SIZE)KEY_STK_SIZE/10,	
                 (CPU_STK_SIZE)KEY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//退出临界区
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}
void key_task(void *p_arg)
{
	OS_ERR err;
	u8 key,size,*rdata;
	

	while(1)
	{
		key = KEY_Scan(0);
	
		switch(key)
		{
			case KEY0_PRES:	LED0 = !LED0;	break;
			case KEY2_PRES: LED1 = !LED1;	break;
			case KEY1_PRES:
				//发送消息
				OSQPost((OS_Q*		)&KEY_Msg,	
						(void*		)data,   //发送消息的内容
						(OS_MSG_SIZE)sizeof(data),
						(OS_OPT		)OS_OPT_POST_FIFO,
						(OS_ERR*	)&err);
				break;
			case WKUP_PRES: 
				//请求消息KEY_Msg
			rdata=OSQPend((OS_Q*			)&KEY_Msg,   
							(OS_TICK		)0,
							(OS_OPT			)OS_OPT_PEND_BLOCKING,
							(OS_MSG_SIZE*	)&size,//储存接收到消息的大小（字节）		
							(CPU_TS*		)0,
							(OS_ERR*		)&err);			
			printf("%s\r\n%d\n\n",rdata,size);
			/*打印请求的消息内容*/
				break;
		}
	}
}

