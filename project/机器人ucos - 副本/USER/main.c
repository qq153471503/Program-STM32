#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "servo.h"

//UCOSIII中以下优先级用户程序不能使用，
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

/*===========================================  ??????  ============================================*/
#define START_TASK_PRIO		3
#define START_STK_SIZE 		64
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

#define WAIST_LEFT_TASK_PRIO 4
#define WAIST_LEFT_STK_SIZE 64
OS_TCB WAIST_LEFT_TASK_TCB;
CPU_STK WAIST_LEFT_TASK_STK[WAIST_LEFT_STK_SIZE];
void waist_left_task(void *p_arg);

#define WAIST_RIGHT_TASK_PRIO 5
#define WAIST_RIGHT_STK_SIZE 64
OS_TCB WAIST_RIGHT_TASK_TCB;
CPU_STK WAIST_RIGHT_TASK_STK[WAIST_LEFT_STK_SIZE];
void waist_right_task(void *p_arg);

#define KNEE_LEFT_TASK_PRIO 6
#define KNEE_LEFT_STK_SIZE 64
OS_TCB KNEE_LEFT_TASK_TCB;
CPU_STK KNEE_LEFT_TASK_STK[KNEE_LEFT_STK_SIZE];
void knee_left_task(void *p_arg);

#define KNEE_RIGHT_TASK_PRIO 7
#define KNEE_RIGHT_STK_SIZE 64
OS_TCB KNEE_RIGHT_TASK_TCB;
CPU_STK KNEE_RIGHT_TASK_STK[KNEE_RIGHT_STK_SIZE];
void knee_right_task(void *p_arg);

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	Servo_PWM(2000,719);
	
	OSInit(&err);		    //???UCOSIII
	OS_CRITICAL_ENTER();	//?????			 
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
	OSStart(&err);      //??UCOSIII
}
//????????
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

	OS_CRITICAL_ENTER();	//????? ???????
	//??led0??
//	OSTaskCreate((OS_TCB 	* )&WAIST_LEFT_TASK_TCB,		
//				 (CPU_CHAR	* )"waist_left_task", 		
//                 (OS_TASK_PTR )waist_left_task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )WAIST_LEFT_TASK_PRIO,     
//                 (CPU_STK   * )&WAIST_LEFT_TASK_STK[0],	
//                 (CPU_STK_SIZE)WAIST_LEFT_STK_SIZE/10,	
//                 (CPU_STK_SIZE)WAIST_LEFT_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,					
//                 (void   	* )0,					
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//                 (OS_ERR 	* )&err);
	OSTaskCreate((OS_TCB 	* )&WAIST_RIGHT_TASK_TCB,		
				 (CPU_CHAR	* )"waist_right_task", 		
                 (OS_TASK_PTR )waist_right_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )WAIST_RIGHT_TASK_PRIO,     
                 (CPU_STK   * )&WAIST_RIGHT_TASK_STK[0],	
                 (CPU_STK_SIZE)WAIST_RIGHT_STK_SIZE/10,	
                 (CPU_STK_SIZE)WAIST_RIGHT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	OSTaskCreate((OS_TCB 	* )&KNEE_LEFT_TASK_TCB,		
				 (CPU_CHAR	* )"knee_left_task", 		
                 (OS_TASK_PTR )knee_left_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )KNEE_LEFT_TASK_PRIO,     
                 (CPU_STK   * )&KNEE_LEFT_TASK_STK[0],	
                 (CPU_STK_SIZE)KNEE_LEFT_STK_SIZE/10,	
                 (CPU_STK_SIZE)KNEE_LEFT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
//	OSTaskCreate((OS_TCB 	* )&KNEE_RIGHT_TASK_TCB,		
//				 (CPU_CHAR	* )"knee_right_task", 		
//                 (OS_TASK_PTR )knee_right_task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )KNEE_RIGHT_TASK_PRIO,     
//                 (CPU_STK   * )&KNEE_RIGHT_TASK_STK[0],	
//                 (CPU_STK_SIZE)KNEE_RIGHT_STK_SIZE/10,	
//                 (CPU_STK_SIZE)KNEE_RIGHT_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,					
//                 (void   	* )0,					
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//?????
	OSTaskDel((OS_TCB*)0,&err);	//??start_task????				 
}

void waist_left_task(void *p_arg)
{
	u16 value = 150;
	u8 flag = 1;
	OS_ERR err;
	TIM_SetCompare1(TIM2,150);
	OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err);	
	while(1)
	{
		TIM_SetCompare1(TIM2,value);
		delay_ms(10);
		
		if(value == 150)
			flag = 1;
		if(value == 175)
			flag = 0;
		
		if(flag)
			value++;
		else 
			value--;
	}
}
void waist_right_task(void *p_arg)
{
	OS_ERR err;
	u16 value = 150;
	u8 flag = 1;
	TIM_SetCompare2(TIM2,150);
	OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err);	
	while(1)
	{
		TIM_SetCompare2(TIM2,value);
		delay_ms(10);
		
		if(value == 200)
			flag = 0;
		if(value == 100)
			flag = 1;
		
		if(flag)
			value++;
		else 
			value--;
	}
}
void knee_left_task(void *p_arg)
{
	OS_ERR err;
	u16 value = 150;
	u8 flag = 1;
	TIM_SetCompare3(TIM2,150);
	OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err);	
	while(1)
	{
		TIM_SetCompare3(TIM2,value);
		delay_ms(10);
		
		if(value == 200)
			flag = 0;
		if(value == 100)
			flag = 1;
		
		if(flag)
			value++;
		else 
			value--;
	}
}
void knee_right_task(void *p_arg)
{
	u16 value = 150;
	u8 flag = 1;
	OS_ERR err;
	TIM_SetCompare4(TIM2,150);
	OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err);	
	while(1)
	{
		TIM_SetCompare4(TIM2,value);
		delay_ms(10);
		
		if(value == 125)
			flag = 1;
		if(value == 150)
			flag = 0;
		
		if(flag)
			value++;
		else 
			value--;
	}
}
