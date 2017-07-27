/************************************************************************************************
��Ϣ����ʵ�� 
����KEY1 ������Ϣ
����wk_up ������Ϣ �������󵽵���Ϣ��ӡ������
************************************************************************************************/

//UCOSIII���������ȼ��û�������ʹ�ã�
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "led.h"
#include "key.h"
#include "beep.h"

//��ʼ����
#define START_TASK_PRIO 3
#define START_STK_SIZE 128
CPU_STK START_TASK_STK[START_STK_SIZE];
OS_TCB START_TASK_TCB;
void start_task(void *p_arg);

//��������
#define KEY_TASK_PRIO 4
#define KEY_STK_SIZE 128
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
OS_TCB KEY_RASK_TCB;
void key_task(void *p_arg);

//������Ϣ����
OS_Q KEY_Msg;


u8 data[10] = "1234567890";

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	LED_Init();
	BEEP_Init();
	KEY_Init();
	
	OSInit(&err);
	OS_CRITICAL_ENTER();	//�����ٽ���	
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&START_TASK_TCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
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
	//������Ϣ����KEY_Msg
	OSQCreate ((OS_Q*		)&KEY_Msg,	//��Ϣ����
                (CPU_CHAR*	)"KEY Msg",	//��Ϣ��������
                (OS_MSG_QTY	)4,	//��Ϣ���г��ȣ���������Ϊ4
                (OS_ERR*	)&err);		//������
	//������������
	OSTaskCreate((OS_TCB 	* )&KEY_RASK_TCB,		
				 (CPU_CHAR	* )"key_task", 		
                 (OS_TASK_PTR )key_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )KEY_TASK_PRIO,     
                 (CPU_STK   * )&KEY_TASK_STK[0],	
                 (CPU_STK_SIZE)KEY_STK_SIZE/10,	
                 (CPU_STK_SIZE)KEY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}
void key_task(void *p_arg)
{
	OS_ERR err;
	u8 key,size,*rdata;
	

	while(1)
	{
		key = KEY_Scan(0);
	
		switch(key)
		{
			case KEY0_PRES:	LED0 = !LED0;	break;
			case KEY2_PRES: LED1 = !LED1;	break;
			case KEY1_PRES:
				//������Ϣ
				OSQPost((OS_Q*		)&KEY_Msg,	
						(void*		)data,   //������Ϣ������
						(OS_MSG_SIZE)sizeof(data),
						(OS_OPT		)OS_OPT_POST_FIFO,
						(OS_ERR*	)&err);
				break;
			case WKUP_PRES: 
				//������ϢKEY_Msg
			rdata=OSQPend((OS_Q*			)&KEY_Msg,   
							(OS_TICK		)0,
							(OS_OPT			)OS_OPT_PEND_BLOCKING,
							(OS_MSG_SIZE*	)&size,//������յ���Ϣ�Ĵ�С���ֽڣ�		
							(CPU_TS*		)0,
							(OS_ERR*		)&err);			
			printf("%s\r\n%d\n\n",rdata,size);
			/*��ӡ�������Ϣ����*/
				break;
		}
	}
}

