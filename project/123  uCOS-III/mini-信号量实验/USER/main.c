#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "lcd.h"
#include "led.h"
#include "key.h"

//UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

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

//LCDˢ��ʱ����ɫ
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
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start_task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII
}
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif
	
	OS_CRITICAL_ENTER();
	OSSemCreate ((OS_SEM*	)&MySem,
                 (CPU_CHAR*	)"MySem",
                 (OS_SEM_CTR)1,		
                 (OS_ERR*	)&err);
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
	//����TASK1����
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
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}
//����1��������
void task1_task(void *p_arg)
{
	OS_ERR err;
	u8 task1_str[]="Task1 Running!";
	while(1)
	{
		printf("\r\n����1:\r\n");
		LCD_Fill(0,110,239,319,BLACK);
		OSSemPend(&MySem,0,OS_OPT_PEND_BLOCKING,0,&err); 	//�����ź���
		memcpy(SemBuf,task1_str,sizeof(task1_str)); //������Դ����������
		delay_ms(300);
		printf("%s\r\n",SemBuf);	//�������������Դ������	
		OSSemPost (&MySem,OS_OPT_POST_1,&err);				//�����ź���
		LED0 = ~LED0;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ1s
	}
}
//����1��������
void task2_task(void *p_arg)
{
	OS_ERR err;
	u8 task2_str[]="Task2 Running!";
	while(1)
	{
		printf("\r\n����2:\r\n");
		LCD_Fill(0,110,239,319,YELLOW);
		OSSemPend(&MySem,0,OS_OPT_PEND_BLOCKING,0,&err); 	//�����ź���
		memcpy(SemBuf,task2_str,sizeof(task2_str)); //������Դ����������
		delay_ms(300);
		printf("%s\r\n",SemBuf);	//�������������Դ������	
		OSSemPost (&MySem,OS_OPT_POST_1,&err);				//�����ź���
		LED1 = ~LED1;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ1s
	}
}
