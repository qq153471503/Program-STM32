#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "lcd.h"
#include "led.h"
#include "key.h"

//UCOSIII���������ȼ��û�������ʹ�ã�
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

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

#define KYE0_FLAG 0X01
#define KEY1_FLAG 0X02
#define KEYFLAG_VALUE 0X00

//����һ���¼���־��
OS_FLAG_GRP EVENTFLAGS;

//LCDˢ��ʱʹ�õ���ɫ
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	KEY_Init();
	LED_Init();
	LCD_Init();

	
	OSInit(&err);		    //��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	//�����ٽ���			 
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&START_TASK_TCB,		//������ƿ�
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
		
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����һ���¼���־��
	OSFlagCreate((OS_FLAG_GRP*)&EVENTFLAGS,		//ָ���¼���־��
                 (CPU_CHAR*	  )"EVENTFLAGS",	//����
                 (OS_FLAGS	  )KEYFLAG_VALUE,	//�¼���־���ʼֵ
                 (OS_ERR*  	  )&err);			//������
	//��������1
	OSTaskCreate((OS_TCB*     )&TASK1_TASK_TCB,		
				 (CPU_CHAR*   )"task1_task", 		
                 (OS_TASK_PTR )task1_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK*    )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//��������2
	OSTaskCreate((OS_TCB*     )&TASK2_TASK_TCB,		
				 (CPU_CHAR*   )"task2_task", 		
                 (OS_TASK_PTR )task2_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TASK2_TASK_PRIO,     
                 (CPU_STK*    )&TASK2_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}

void task1_task(void *p_arg)
{
	OS_ERR err;
	u8 num;
	OS_FLAGS flags_num;
	
	while(1)
	{	
		if(KEY0 == 0)
		{
			//���¼���־��EventFlags���ͱ�־
			flags_num=OSFlagPost((OS_FLAG_GRP*)&EVENTFLAGS,
								 (OS_FLAGS	  )KYE0_FLAG,
								 (OS_OPT	  )OS_OPT_POST_FLAG_SET,
					             (OS_ERR*	  )&err);
			LCD_ShowNum(174,110,flags_num,1,16);
			printf("�¼���־��EventFlags��ֵ:%d\r\n",flags_num);
			while(!KEY0);
		}else if(KEY1 == 0)
		{
			//���¼���־��EventFlags���ͱ�־
			flags_num=OSFlagPost((OS_FLAG_GRP*)&EVENTFLAGS,
								 (OS_FLAGS	  )KEY1_FLAG,
								 (OS_OPT	  )OS_OPT_POST_FLAG_SET,
								 (OS_ERR*     )&err);
			LCD_ShowNum(174,110,flags_num,1,16);
			printf("�¼���־��EventFlags��ֵ:%d\r\n",flags_num);
			while(!KEY1);
		}
		num++;
		if(num == 50)
		{
			num =0;
			LED0 = !LED0;
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //��ʱ10ms
	}
}
void task2_task(void *p_arg)
{
	u8 num;
	OS_ERR err;
	while(1)
	{
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&EVENTFLAGS,
				   (OS_FLAGS	)KYE0_FLAG+KEY1_FLAG,
		     	   (OS_TICK     )0,
				   (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_BLOCKING,
				   (CPU_TS*     )0,
				   (OS_ERR*	    )&err);
		num++;
		LED1=!LED1;
		LCD_Fill(6,131,233,313,lcd_discolor[num%14]);
		LCD_ShowNum(174,130 ,EVENTFLAGS.Flags,1,16);
		printf("�¼���־��EventFlags��ֵ:%d\r\n",EVENTFLAGS.Flags);
		LCD_ShowNum(174,110,EVENTFLAGS.Flags,1,16);
	}
}

