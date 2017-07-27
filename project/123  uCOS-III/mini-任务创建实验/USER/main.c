#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "includes.h"

//UCOSIII中以下优先级用户程序不能使用，
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

/*===========================================  开始任务配置  ============================================*/
#define START_TASK_PRIO		3
#define START_STK_SIZE 		128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);
/*===========================================  LED0任务配置 ============================================*/
#define LED0_TASK_PRIO		4	
#define LED0_STK_SIZE 		128
OS_TCB LED0TaskTCB;
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);
/*===========================================  LED1任务配置 ============================================*/
#define LED1_TASK_PRIO		5
#define LED1_STK_SIZE 		128
OS_TCB LED1TaskTCB;
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
void led1_task(void *p_arg);
/*===========================================  LCD任务一任务配置 ============================================*/
#define LCD1_TASK_PRIO	6
#define LCD1_STK_SIZE	128
OS_TCB LCD1TaskTCB;
CPU_STK LCD1_TASK_STK[LCD1_STK_SIZE];
void lcd1_task(void *p_arg);
/*===========================================  LCD任务二任务配置 ============================================*/
#define LCD2_TASK_PRIO	7
#define LCD2_STK_SIZE	128
OS_TCB LCD2TaskTCB;
CPU_STK LCD2_TASK_STK[LCD1_STK_SIZE];
void lcd2_task(void *p_arg);
/*===========================================  LCD任务三任务配置 ============================================*/
#define LCD3_TASK_PRIO	8
#define LCD3_STK_SIZE	128
OS_TCB LCD3TaskTCB;
CPU_STK LCD3_TASK_STK[LCD3_STK_SIZE];
void lcd3_task(void *p_arg);
/*******************************************************************************************************************/


u32 color[14] = {WHITE,BLACK,BLUE,BRED,GRED,GBLUE,RED,MAGENTA,GREEN,CYAN,YELLOW,BROWN,BRRED,GRAY};

//主函数
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();  //时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	uart_init(115200);   //串口初始化
	LED_Init();         //LED初始化	
	LCD_Init();
	
	POINT_COLOR = RED;
	LCD_ShowString(50,30,"uC/OS-III");
	LCD_ShowString(50,50,"Tasks create and delete test");
	OSInit(&err);		    //初始化UCOSIII
	OS_CRITICAL_ENTER();	//进入临界区			 
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
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

//开始任务任务函数
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
	
	OS_CRITICAL_ENTER();	//进入临界区 无法被中断打断
	//创建led0任务
	OSTaskCreate((OS_TCB 	* )&LED0TaskTCB,		
				 (CPU_CHAR	* )"led0_ask", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
	//创建led1任务
	OSTaskCreate((OS_TCB 	* )&LED1TaskTCB,		
				 (CPU_CHAR	* )"led1_task", 		
                 (OS_TASK_PTR )led1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED1_TASK_PRIO,     	
                 (CPU_STK   * )&LED1_TASK_STK[0],	
                 (CPU_STK_SIZE)LED1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
	//创建lcd1任务			 
	OSTaskCreate((OS_TCB 	* )&LCD1TaskTCB,		
				 (CPU_CHAR	* )"lcd1_task", 		
                 (OS_TASK_PTR )lcd1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LCD1_TASK_PRIO,     	
                 (CPU_STK   * )&LCD1_TASK_STK[0],	
                 (CPU_STK_SIZE)LCD1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LCD1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
	//创建lcd2任务			 
	OSTaskCreate((OS_TCB 	* )&LCD2TaskTCB,		
				 (CPU_CHAR	* )"lcd2_task", 		
                 (OS_TASK_PTR )lcd2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LCD2_TASK_PRIO,     	
                 (CPU_STK   * )&LCD2_TASK_STK[0],	
                 (CPU_STK_SIZE)LCD2_STK_SIZE/10,	
                 (CPU_STK_SIZE)LCD2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
	//创建lcd3任务			 
	OSTaskCreate((OS_TCB 	* )&LCD3TaskTCB,		
				 (CPU_CHAR	* )"lcd3_task", 		
                 (OS_TASK_PTR )lcd3_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LCD3_TASK_PRIO,     	
                 (CPU_STK   * )&LCD3_TASK_STK[0],	
                 (CPU_STK_SIZE)LCD3_STK_SIZE/10,	
                 (CPU_STK_SIZE)LCD3_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 
	OS_CRITICAL_EXIT();	//退出临界区
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
//	OSTaskSuspend((OS_TCB*)&StartTaskTCB,&err);
	while(1);
}
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		LED1 = 0;
		OSTimeDlyHMSM(0,0,0,700,OS_OPT_TIME_HMSM_STRICT,&err);
		LED1 = 1;
		OSTimeDlyHMSM(0,0,1,350,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
void lcd1_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	u8 num = 0;
	u32 count = 0;
	p_arg = p_arg;
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	LCD_DrawRectangle(40,80,146,200);
	LCD_DrawLine(40,100,146,100);
	POINT_COLOR = BLUE;
	LCD_ShowString(41,81," LCD1 Run:");
	OS_CRITICAL_EXIT();
	while(1)
	{
		printf("LCD1任务已经运行%d次\r\n",num);
		if(num == 5)
		{
			printf("\n\nLCD2任务挂起了LCD1任务！\r\n\n");
			OSTaskSuspend((OS_TCB *)&LCD2TaskTCB,&err);
//			OSTaskDel((OS_TCB *)&LCD1TaskTCB,&err);
		}
		if(num == 10)
		{
			num = 0;
			printf("\n\nLCD2任务解挂了LCD1任务！\r\n\n");
			OSTaskResume((OS_TCB *)&LCD2TaskTCB,&err);
		}
		LCD_Fill(41,101,145,199,color[count%14]);
		LCD_ShowNum(128,81,count,2,16);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 
		num++;
		count++;
	}
}
void lcd2_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	u32 num = 0;
	p_arg = p_arg;
	
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	LCD_DrawRectangle(150,80,256,200);
	LCD_DrawLine(150,100,256,100);
	POINT_COLOR = BLUE;
	
	LCD_ShowString(151,81, "LCD2 Run:");
	
	OS_CRITICAL_EXIT();
	
	while(1)
	{
		printf("LCD2任务已经运行%d次\r\n",num);
		LCD_ShowNum(232,81,num,2,16);
		LCD_Fill(151,101,255,199,color[num%14]);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
		num++;		
	}
}
void lcd3_task(void *p_arg)
{
	OS_ERR err;
	u8 num = 0;
	p_arg = p_arg;

	while(1)
	{
		LCD_Fill(0,0,320,20,color[num%14]);/*横上*/
		LCD_Fill(0,0,20,240,color[num%14]);/*竖左*/
		LCD_Fill(0,220,320,240,color[num%14]);/*横下*/
		LCD_Fill(300,0,320,240,color[num%14]);/*竖右*/
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500毫秒
		num++;
	}
}

