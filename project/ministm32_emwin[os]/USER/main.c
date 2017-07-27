#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "adc.h"
#include "touch.h"
#include "ili93xx.h"
#include "gui.h"
#include "wm.h"
#include "includes.h"
#include "demo.h"

//¿ªÊ¼ÈÎÎñ
#define START_TASK_PRIO		3	
#define START_STK_SIZE 		128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

//led0ÈÎÎñ
#define LED0_TASK_PRIO		4
#define LED0_STK_SIZE 		64
OS_TCB Led0TaskTCB;
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//led1ÈÎÎñ
#define LED1_TASK_PRIO		5
#define LED1_STK_SIZE 		64
OS_TCB Led1TaskTCB;
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
void led1_task(void *p_arg);

//lcdÏÔÊ¾ÈÎÎñ
#define LCD_TASK_PRIO	6
#define LCD_STK_SIZE	768
OS_TCB LCD_TAST_TCB;
CPU_STK LCD_TASK_STK[LCD_STK_SIZE];
void lcd_task(void *pdata);

int main(void)
{
	 OS_ERR err;
	 CPU_SR_ALLOC();
	//SystemInit();			//系统初始化 可不写 在汇编启动文件中 main函数之前已执行
	delay_init();			//延时初始化
	uart_init(115200);		//串口1初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断分组2 两位抢占 两位响应
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
	LED_Init();
	KEY_Init();
	Adc_Init();
	TFTLCD_Init();
	
	OSInit(&err);		
	OS_CRITICAL_ENTER();

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
//	WM_SetCreateFlags(WM_CF_MEMDEV);
//	GUI_Init();
//	GUI_SetFont(&GUI_Font32B_ASCII);
//	GUI_SetBkColor(GUI_BLACK);
//	GUI_SetColor(GUI_YELLOW);
//	GUI_Clear();
//	GUI_DispStringAt("Hello World !", 0, 0);
//	while(1)
//	{
//		
//	}
}
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
	
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//Í³¼ÆÈÎÎñ                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//Èç¹ûÊ¹ÄÜÁË²âÁ¿ÖÐ¶Ï¹Ø±ÕÊ±¼ä
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //µ±Ê¹ÓÃÊ±¼äÆ¬ÂÖ×ªµÄÊ±ºò
	 //Ê¹ÄÜÊ±¼äÆ¬ÂÖ×ªµ÷¶È¹¦ÄÜ,Ê±¼äÆ¬³¤¶ÈÎª1¸öÏµÍ³Ê±ÖÓ½ÚÅÄ£¬¼È1*5=5ms
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
	pdata = pdata;
	mainTask();
}


