#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "key.h"
#include "led.h"
#include "lcd.h"

//UCOSIII���������ȼ��û�������ʹ�ã�
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

//LCDˢ��ʱʹ�õ���ɫ
int lcd_discolor[14]={	WHITE, BLACK, BLUE,  BRED,      
						GRED,  GBLUE, RED,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };

//��ʼ����
OS_TCB StartTaskTCB;
#define START_TASK_PRIO 3
#define START_STK_SIZE 128
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

//����1
OS_TCB Task1TaskTCB;
#define TASK1_TASK_PRIO 4
#define TASK1_STK_SIZE 128
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

//����2
OS_TCB Task2TaskTCB;
#define TASK2_TASK_PRIO 5
#define TASK2_STK_SIZE 128
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
void task2_task(void *p_arg);

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	LED_Init();
	LCD_Init();
	KEY_Init();
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,234,314);	
	LCD_DrawLine(5,130,234,130);
	POINT_COLOR = RED;
	LCD_ShowString(30,111,200,16,16,"Task_Sem Value:  0");
	POINT_COLOR = BLUE;
	
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
                 (OS_ERR 	* )&err);	
//	OSTaskSemSet();
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
	//��������1
	OSTaskCreate((OS_TCB 	* )&Task1TaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"task1_task", 		//��������
                 (OS_TASK_PTR )task1_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )TASK1_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&TASK1_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);	
	//��������2
	OSTaskCreate((OS_TCB 	* )&Task2TaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"task2_task", 		//��������
                 (OS_TASK_PTR )task2_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )TASK2_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&TASK2_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskSuspend((OS_TCB *)0,&err);/*������������*/
}
void task1_task(void *p_arg)
{
	OS_ERR err;
	u8 key;
	u8 num;
	
	while(1)
	{
		key = KEY_Scan(0);
		if(key == WKUP_PRES)
		{
			OSTaskSemPost(&Task2TaskTCB,OS_OPT_POST_NONE,&err);	
			LCD_ShowxNum(150,111,Task2TaskTCB.SemCtr,3,16,0);		//��ʾ�ź���ֵ
		}
		num++;
		if(num == 50)
		{
			num = 0;
			LED0 = !LED0;
		}
		delay_ms(10);
	}
}
void task2_task(void *p_arg)
{
	u8 num;
	OS_ERR err;
	
	while(1)
	{
		num++;
		OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
		LCD_ShowxNum(150,111,Task2TaskTCB.SemCtr,3,16,0);	//��ʾ�����ڽ��ź���ֵ
		LCD_Fill(6,131,233,313,lcd_discolor[num%14]);		//ˢ��
		LED1 = ~LED1;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);   //��ʱ1s
	}
}
