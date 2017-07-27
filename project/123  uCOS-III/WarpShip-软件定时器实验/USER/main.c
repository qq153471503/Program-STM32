#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"

//UCOSIII���������ȼ��û�������ʹ�ã�
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

#define START_TASK_PRIO 3
#define START_STK_SIZE	128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

#define TASK1_TASK_PRIO 3
#define TASK1_STK_SIZE	128
OS_TCB Task1TaskTCB;
CPU_STK TASK1_TASK_STK[START_STK_SIZE];
void task1_task(void *p_arg);

OS_TMR timer1;
OS_TMR timer2;
void timer1_callback(void *P_tmr,void *p_arg);
void timer2_callback(void *P_tmr,void *p_arg);

int lcd_color[14]={	WHITE, RED,   BLUE,  BRED,      
						GRED,  GBLUE, BLACK,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };

						
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	LED_Init();
	LCD_Init();
	BEEP_Init();
	KEY_Init();
	
	LCD_DrawLine(0,108,239,108);		
	LCD_DrawLine(119,108,119,319);		
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,115,314); 		
	LCD_DrawLine(5,130,115,130);		
	
	LCD_DrawRectangle(125,110,234,314); 	
	LCD_DrawLine(125,130,234,130);		
	POINT_COLOR = BLUE;
	LCD_ShowString(6,111,110,16,16,	 "TIMER1:000");
	LCD_ShowString(126,111,110,16,16,"TIMER2:000");
	
	OSInit(&err);
	OS_CRITICAL_ENTER();
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		
				 (CPU_CHAR	* )"start_task", 		
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
   OSStatTaskCPUUsageInit(&err);  	                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif	
	
	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&timer1,		//��ʱ��1
                (CPU_CHAR	*)"timer1",		//��ʱ������
                (OS_TICK	 )20,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)timer1_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����
	//������ʱ��2
	OSTmrCreate((OS_TMR		*)&timer2,		
                (CPU_CHAR	*)"timer2",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//���ζ�ʱ��
                (OS_TMR_CALLBACK_PTR)timer2_callback,	//��ʱ��2�ص�����
                (void	    *)0,			
                (OS_ERR	    *)&err);
				
	OS_CRITICAL_ENTER();
	//����TASK1����
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
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������			
}
void task1_task(void *p_arg)
{
	u8 key,num;
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		key = KEY_Scan(0);
		
		switch(key)
		{
			case WKUP_PRES:
				OSTmrStart(&timer1,&err);
				printf("�����˶�ʱ��1����\n");break;
			case KEY0_PRES:
				OSTmrStart(&timer2,&err);
				printf("�����˶�ʱ��2����\n");break;
			case KEY1_PRES:
				OSTmrStop(&timer1,OS_OPT_TMR_NONE,0,&err);
				OSTmrStop(&timer2,OS_OPT_TMR_NONE,0,&err);
				printf("�����˶�ʱ��1����\n");break;
		}
		num++;
		
		if(num>=50)
		{
			num = 0;
			LED0 = !LED0;
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
	}
}
void timer1_callback(void *P_tmr,void *p_arg)
{
	static u8 count = 0;
	LCD_ShowxNum(62,111,count,3,16,0x80); //��ʾ��ʱ��1��ִ�д���
	LCD_Fill(6,131,114,313,lcd_color[count%14]);//�������
	count++;	
}
void timer2_callback(void *P_tmr,void *p_arg)
{
	static u8 count = 0;
	count++;
	LCD_ShowxNum(182,111,count,3,16,0x80);  //��ʾ��ʱ��1ִ�д���
	LCD_Fill(126,131,233,313,lcd_color[count%14]); //�������
	LED1 = ~LED1;
	printf("��ʱ��2���н���\r\n");	
}

