#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "includes.h"

//UCOSIII���������ȼ��û�������ʹ�ã�
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

/*===========================================  ��ʼ��������  ============================================*/
#define START_TASK_PRIO		3
#define START_STK_SIZE 		128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);
/*===========================================  LED0�������� ============================================*/
#define LED0_TASK_PRIO		4	
#define LED0_STK_SIZE 		128
OS_TCB LED0TaskTCB;
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);
/*===========================================  LED1�������� ============================================*/
#define LED1_TASK_PRIO		5
#define LED1_STK_SIZE 		128
OS_TCB LED1TaskTCB;
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
void led1_task(void *p_arg);
/*===========================================  LCD����һ�������� ============================================*/
#define LCD1_TASK_PRIO	6
#define LCD1_STK_SIZE	128
OS_TCB LCD1TaskTCB;
CPU_STK LCD1_TASK_STK[LCD1_STK_SIZE];
void lcd1_task(void *p_arg);
/*===========================================  LCD������������� ============================================*/
#define LCD2_TASK_PRIO	7
#define LCD2_STK_SIZE	128
OS_TCB LCD2TaskTCB;
CPU_STK LCD2_TASK_STK[LCD1_STK_SIZE];
void lcd2_task(void *p_arg);
/*===========================================  LCD�������������� ============================================*/
#define LCD3_TASK_PRIO	8
#define LCD3_STK_SIZE	128
OS_TCB LCD3TaskTCB;
CPU_STK LCD3_TASK_STK[LCD3_STK_SIZE];
void lcd3_task(void *p_arg);
/*******************************************************************************************************************/


u32 color[14] = {WHITE,BLACK,BLUE,BRED,GRED,GBLUE,RED,MAGENTA,GREEN,CYAN,YELLOW,BROWN,BRRED,GRAY};

//������
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();  //ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	uart_init(115200);   //���ڳ�ʼ��
	LED_Init();         //LED��ʼ��	
	LCD_Init();
	
	POINT_COLOR = RED;
	LCD_ShowString(50,30,"uC/OS-III");
	LCD_ShowString(50,50,"Tasks create and delete test");
	OSInit(&err);		    //��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	//�����ٽ���			 
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
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

//��ʼ����������
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
	
	OS_CRITICAL_ENTER();	//�����ٽ��� �޷����жϴ��
	//����led0����
	OSTaskCreate((OS_TCB 	* )&LED0TaskTCB,		
				 (CPU_CHAR	* )"led0_ask", 		
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
				 
	//����led1����
	OSTaskCreate((OS_TCB 	* )&LED1TaskTCB,		
				 (CPU_CHAR	* )"led1_task", 		
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
	//����lcd1����			 
	OSTaskCreate((OS_TCB 	* )&LCD1TaskTCB,		
				 (CPU_CHAR	* )"lcd1_task", 		
                 (OS_TASK_PTR )lcd1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LCD1_TASK_PRIO,     	
                 (CPU_STK   * )&LCD1_TASK_STK[0],	
                 (CPU_STK_SIZE)LCD1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LCD1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
	//����lcd2����			 
	OSTaskCreate((OS_TCB 	* )&LCD2TaskTCB,		
				 (CPU_CHAR	* )"lcd2_task", 		
                 (OS_TASK_PTR )lcd2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LCD2_TASK_PRIO,     	
                 (CPU_STK   * )&LCD2_TASK_STK[0],	
                 (CPU_STK_SIZE)LCD2_STK_SIZE/10,	
                 (CPU_STK_SIZE)LCD2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
	//����lcd3����			 
	OSTaskCreate((OS_TCB 	* )&LCD3TaskTCB,		
				 (CPU_CHAR	* )"lcd3_task", 		
                 (OS_TASK_PTR )lcd3_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LCD3_TASK_PRIO,     	
                 (CPU_STK   * )&LCD3_TASK_STK[0],	
                 (CPU_STK_SIZE)LCD3_STK_SIZE/10,	
                 (CPU_STK_SIZE)LCD3_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
//	OSTaskSuspend((OS_TCB*)&StartTaskTCB,&err);
	while(1);
}
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		LED1 = 0;
		OSTimeDlyHMSM(0,0,0,700,OS_OPT_TIME_HMSM_STRICT,&err);
		LED1 = 1;
		OSTimeDlyHMSM(0,0,1,350,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
void lcd1_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	u8 num = 0;
	u32 count = 0;
	p_arg = p_arg;
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	LCD_DrawRectangle(40,80,146,200);
	LCD_DrawLine(40,100,146,100);
	POINT_COLOR = BLUE;
	LCD_ShowString(41,81," LCD1 Run:");
	OS_CRITICAL_EXIT();
	while(1)
	{
		printf("LCD1�����Ѿ�����%d��\r\n",num);
		if(num == 5)
		{
			printf("\n\nLCD2���������LCD1����\r\n\n");
			OSTaskSuspend((OS_TCB *)&LCD2TaskTCB,&err);
//			OSTaskDel((OS_TCB *)&LCD1TaskTCB,&err);
		}
		if(num == 10)
		{
			num = 0;
			printf("\n\nLCD2��������LCD1����\r\n\n");
			OSTaskResume((OS_TCB *)&LCD2TaskTCB,&err);
		}
		LCD_Fill(41,101,145,199,color[count%14]);
		LCD_ShowNum(128,81,count,2,16);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); 
		num++;
		count++;
	}
}
void lcd2_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	u32 num = 0;
	p_arg = p_arg;
	
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	LCD_DrawRectangle(150,80,256,200);
	LCD_DrawLine(150,100,256,100);
	POINT_COLOR = BLUE;
	
	LCD_ShowString(151,81, "LCD2 Run:");
	
	OS_CRITICAL_EXIT();
	
	while(1)
	{
		printf("LCD2�����Ѿ�����%d��\r\n",num);
		LCD_ShowNum(232,81,num,2,16);
		LCD_Fill(151,101,255,199,color[num%14]);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
		num++;		
	}
}
void lcd3_task(void *p_arg)
{
	OS_ERR err;
	u8 num = 0;
	p_arg = p_arg;

	while(1)
	{
		LCD_Fill(0,0,320,20,color[num%14]);/*����*/
		LCD_Fill(0,0,20,240,color[num%14]);/*����*/
		LCD_Fill(0,220,320,240,color[num%14]);/*����*/
		LCD_Fill(300,0,320,240,color[num%14]);/*����*/
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500����
		num++;
	}
}

