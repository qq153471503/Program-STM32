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

OS_SEM sem1;
OS_SEM sem2;
OS_Q q_queue;
#define QUEUE_NUM 10
#define CORE_NUM 3

//���ڴ洢��Ϣ���е�����
u8 QUEUE_TABLE[] = "1234567890";

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
	LCD_Init();
	LED_Init();
	KEY_Init();

	POINT_COLOR = RED;
	LCD_ShowString(30,10,200,16,16,"ALIENTEK STM32F1");	
	LCD_ShowString(30,30,200,16,16,"UCOSIII Examp 13-1");
	LCD_ShowString(30,50,200,16,16,"Pend Multi");
	LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,16,16,"2015/5/20");
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,234,314);	
	LCD_DrawLine(5,130,234,130);
	POINT_COLOR = RED;
	LCD_ShowString(50,111,200,16,16,"ObjRdy_NUM: 0");
	POINT_COLOR = BLUE;
	
	OSInit(&err);
	OS_CRITICAL_ENTER();
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
	//�����ź���sem1
	OSSemCreate ((OS_SEM*	)&sem1,
                 (CPU_CHAR*	)"sem1",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);
	//�����ź���sem2
	OSSemCreate ((OS_SEM*	)&sem2,
                 (CPU_CHAR*	)"sem2",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);
	//������Ϣ����
	OSQCreate ((OS_Q*		)&q_queue,	//��Ϣ����
                (CPU_CHAR*	)"q_queue",	//��Ϣ��������
                (OS_MSG_QTY	)QUEUE_NUM,	//��Ϣ���г���
                (OS_ERR*	)&err);		//������
	//����TASK1����
	OSTaskCreate((OS_TCB 	* )&TASK1_TASK_TCB,		
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
	//����TASK2����
	OSTaskCreate((OS_TCB 	* )&TASK2_TASK_TCB,		
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
	OSTaskDel((OS_TCB*)&START_TASK_TCB,&err);	//ɾ��start_task��������				 
}
void task1_task(void *p_arg)
{
	u8 key;
	u8 num = 0;
	OS_ERR err;
	u8 *data;
	u8 size;
	
	while(1)
	{
		key = KEY_Scan(0);		
		switch(key)
		{
			case KEY0_PRES:
					OSSemPost(&sem1,OS_OPT_POST_1,&err);
					break;
			case KEY1_PRES:
					OSSemPost(&sem2,OS_OPT_POST_1,&err);
					break;
			case KEY2_PRES:
				//������Ϣ
				OSQPost((OS_Q*		)&q_queue,		
						(void*		)QUEUE_TABLE,
						(OS_MSG_SIZE)sizeof(QUEUE_TABLE),
						(OS_OPT		)OS_OPT_POST_FIFO,
						(OS_ERR*	)&err);break;
			case WKUP_PRES:
				//������ϢKEY_Msg
			data = OSQPend((OS_Q*			)&q_queue,   
							(OS_TICK		)0,
							(OS_OPT			)OS_OPT_PEND_BLOCKING,
							(OS_MSG_SIZE*	)&size,//������յ���Ϣ�Ĵ�С���ֽڣ�		
							(CPU_TS*		)0,
							(OS_ERR*		)&err);			
			printf("%s\r\n\n\n",data);break;
			/*��ӡ���������Ϣ����*/
		}	
		num++;
		if(num >= 50)
		{
			num = 0;
			LED0 = !LED0;//��led��
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
void task2_task(void *p_arg)
{
	OS_ERR err;
	OS_OBJ_QTY num;
	u8 count;
	OS_PEND_DATA table[QUEUE_NUM];
	
	table[0].PendObjPtr = (OS_PEND_OBJ *)&sem1;
	table[1].PendObjPtr = (OS_PEND_OBJ *)&sem2;
	table[2].PendObjPtr = (OS_PEND_OBJ *)&q_queue;
	
	while(1)
	{
		num = OSPendMulti((OS_PEND_DATA*	)table,	
						  (OS_OBJ_QTY		)CORE_NUM,	//�ں�����
						  (OS_TICK		   	)0,		
						  (OS_OPT         	)OS_OPT_PEND_BLOCKING,
						  (OS_ERR*			)&err);
		LCD_ShowNum(147,111,num,1,16);  					//��ʾ��ǰ�м����ں˶���׼������		
		count++;
		LED1 = !LED1;
		LCD_Fill(6,131,233,313,lcd_discolor[count%14]);		//ˢ��
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
	}	
}

