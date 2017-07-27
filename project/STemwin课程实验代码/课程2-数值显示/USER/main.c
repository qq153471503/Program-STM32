/*>>>
	UCOSIII中以下优先级用户程序不能使用，将这些优先级分配给了UCOSIII的5个系统内部任务
	优先级0：中断服务服务管理任务 OS_IntQTask()
	优先级1：时钟节拍任务 OS_TickTask()
	优先级2：定时任务 OS_TmrTask()
	优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
	优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
<<<*/
	
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "ILI93XX.h"
#include "usart.h"	  
#include "sram.h"
#include "malloc.h"
#include "string.h"
#include "usmart.h"	
#include "GUI.H"
#include "includes.h"


//开始任务
#define START_TASK_PRIO		3	
#define START_STK_SIZE 		128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

//led0任务
#define LED0_TASK_PRIO		4
#define LED0_STK_SIZE 		64
OS_TCB Led0TaskTCB;
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//led1任务
#define LED1_TASK_PRIO		5
#define LED1_STK_SIZE 		64
OS_TCB Led1TaskTCB;
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
void led1_task(void *p_arg);

//lcd显示任务
#define LCD_TASK_PRIO	6
#define LCD_STK_SIZE	512
OS_TCB LCD_TAST_TCB;
CPU_STK LCD_TASK_STK[LCD_STK_SIZE];
void lcd_task(void *pdata);

 int main(void)
 {	 
	 
	 OS_ERR err;
	 CPU_SR_ALLOC();
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	TFTLCD_Init();			   		//初始化LCD   
 	FSMC_SRAM_Init();			//初始化外部SRAM  
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
	
	OSInit(&err);		
	OS_CRITICAL_ENTER();

	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		
				 (CPU_CHAR	* )"start task", 		
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
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif		
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	GUI_Init();
	
	OS_CRITICAL_ENTER();

	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
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

	OSTaskCreate((OS_TCB 	* )&Led1TaskTCB,		
				 (CPU_CHAR	* )"led1 task", 		
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

	OSTaskCreate((OS_TCB 	* )&LCD_TAST_TCB,		
				 (CPU_CHAR	* )"lcd task", 		
                 (OS_TASK_PTR )lcd_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LCD_TASK_PRIO,     	
                 (CPU_STK   * )&LCD_TASK_STK[0],	
                 (CPU_STK_SIZE)LCD_STK_SIZE/10,	
                 (CPU_STK_SIZE)LCD_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err); 
	OS_CRITICAL_EXIT();	
}

void led0_task(void *p_arg)
{
	
	OS_ERR err;
	p_arg = p_arg ;
	
	while(1)
	{
		LED0 = 1;
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_PERIODIC, &err);
		LED0 = 0;
		OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_PERIODIC, &err);
	}
}

void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg ;
	
	while(1)
	{
		LED1 = 0;
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_PERIODIC, &err);
		LED1 = 1;
		OSTimeDlyHMSM(0, 0, 0, 300, OS_OPT_TIME_PERIODIC, &err);
	}
}

void lcd_task(void *pdata)
{
	char *emwin_version;			//用来储存emwin的版本号
	pdata = pdata;
	GUI_SetBkColor(GUI_BLUE);		//设置背景色
	GUI_Clear();					//清屏
	GUI_SetTextMode(GUI_TM_TRANS);	//设置文本模式为透明模式
	GUI_SetFont(&GUI_Font32_ASCII);	//设置字体
	GUI_SetColor(GUI_YELLOW);		//设置前景色
	GUI_DispStringHCenterAt("Show Number Test ",239/2, 0);
	
	GUI_SetFont(&GUI_Font24_ASCII);					//设置字体
	GUI_SetColor(GUI_RED);							//设置前景色
	GUI_DispDecAt(12345, 40, 40, 5);				//指定位置显示十进制数12345
	GUI_GotoXY(40,60);								//移动光标
	GUI_DispFloat(123.45, 6);						//显示正浮点数123.45
	GUI_GotoXY(40,80);
	GUI_DispFloat(-123.45, 7);						//显示负数123.45
	GUI_DispBinAt(10, 40, 100, 4);					//指定位置显示10进制数的 二进制[1010]
	GUI_DispHexAt(17, 40, 120, 4);					//指定位置显示17进制数的 十六进制
	emwin_version = (char *)GUI_GetVersionString();	//获取emwin的版本号
	GUI_DispStringAt("STemwin Version: ", 0, 319-24);
	GUI_DispString(emwin_version);
	
  	while(1)
	{
		//加此延时的目的是为了发起一次任务调度
		GUI_Delay(100);
	}
}


