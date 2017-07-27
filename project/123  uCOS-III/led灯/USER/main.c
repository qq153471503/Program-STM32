#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
/************************************************
 ALIENTEK??STM32???UCOS??
 ?4-1 UCOSIII UCOSIII??
 ????:www.openedv.com
 ????:http://eboard.taobao.com 
 ???????????:"????",????STM32???
 ?????????????  
 ??:???? @ALIENTEK
************************************************/

//UCOSIII??????????????,ALIENTEK
//??????????UCOSIII?5???????
//???0:?????????? OS_IntQTask()
//???1:?????? OS_TickTask()
//???2:???? OS_TmrTask()
//???OS_CFG_PRIO_MAX-2:???? OS_StatTask()
//???OS_CFG_PRIO_MAX-1:???? OS_IdleTask()
//????:www.openedv.com
//????:http://eboard.taobao.com  
//?????????????  
//??:???? @ALIENTEK

//?????
#define START_TASK_PRIO		3
//??????	
#define START_STK_SIZE 		512
//?????
OS_TCB StartTaskTCB;
//????	
CPU_STK START_TASK_STK[START_STK_SIZE];
//????
void start_task(void *p_arg);

//?????
#define LED0_TASK_PRIO		4
//??????	
#define LED0_STK_SIZE 		128
//?????
OS_TCB Led0TaskTCB;
//????	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//?????
#define LED1_TASK_PRIO		5
//??????	
#define LED1_STK_SIZE 		128
//?????
OS_TCB Led1TaskTCB;
//????	
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
//????
void led1_task(void *p_arg);

//?????
#define FLOAT_TASK_PRIO		6
//??????
#define FLOAT_STK_SIZE		128
//?????
OS_TCB	FloatTaskTCB;
//????
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];
//????
void float_task(void *p_arg);

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();       //?????
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //??????
	uart_init(115200);    //???????
	LED_Init();         //LED???
	
	OSInit(&err);		//???UCOSIII
	OS_CRITICAL_ENTER();//?????
	//??????
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//?????
				 (CPU_CHAR	* )"start task", 		//????
                 (OS_TASK_PTR )start_task, 			//????
                 (void		* )0,					//??????????
                 (OS_PRIO	  )START_TASK_PRIO,     //?????
                 (CPU_STK   * )&START_TASK_STK[0],	//???????
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//????????
                 (CPU_STK_SIZE)START_STK_SIZE,		//??????
                 (OS_MSG_QTY  )0,					//???????????????????,?0???????
                 (OS_TICK	  )0,					//???????????????,?0??????,
                 (void   	* )0,					//????????
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //????
                 (OS_ERR 	* )&err);				//????????????
	OS_CRITICAL_EXIT();	//?????	 
	OSStart(&err);  //??UCOSIII
	while(1);
}

//??????
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//????                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//?????????????
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //???????????
	 //???????????,??????1???????,?1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//?????
	//??LED0??
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
				 
	//??LED1??
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
			 
	//????????
	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
				 (CPU_CHAR	* )"float test task", 		
                 (OS_TASK_PTR )float_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//??????			 
	OS_CRITICAL_EXIT();	//?????
}

//led0????
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LED0=0;
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //??200ms
		LED0=1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //??500ms
	}
}

//led1????
void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LED1=~LED1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //??500ms
	}
}

//??????
void float_task(void *p_arg)
{
	CPU_SR_ALLOC();
	static float float_num=0.01;
	while(1)
	{
		float_num+=0.01f;
		OS_CRITICAL_ENTER();	//?????
		printf("float_num???: %.4f\r\n",float_num);
		OS_CRITICAL_EXIT();		//?????
		delay_ms(500);			//??500ms
	}
}
