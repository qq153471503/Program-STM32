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

OS_TMR timer0;
OS_TMR timer1;
void timer0_callback(void *p_tmr,void *p_arg);
void timer1_callback(void *p_tmr,void *p_arg);


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
	LCD_ShowString(30,50,"KEY_UP:Start Tmr1");
	LCD_ShowString(30,70,"KEY0:Start Timer2");
	LCD_ShowString(30,90,"KEY1:Stop Timer1 and Timer2");
	
	LCD_DrawLine(0,108,239,108);		//����
	LCD_DrawLine(119,108,119,319);		
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,115,314); 	//��һ������
	LCD_DrawLine(5,130,115,130);		//����
	
	LCD_DrawRectangle(125,110,234,314); //������
	LCD_DrawLine(125,130,234,130);		//����
	POINT_COLOR = BLUE;
	LCD_ShowString(6,111,"TIMER1:000");
	LCD_ShowString(126,111,"TIMER2:000");
	
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
//��ʼ������
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

	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&timer0,		//��ʱ��1
                (CPU_CHAR	*)"timer0",		//��ʱ������
                (OS_TICK	 )20,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)timer0_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����
	//������ʱ��2
	OSTmrCreate((OS_TMR		*)&timer1,		
                (CPU_CHAR	*)"timer1",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//���ζ�ʱ��
                (OS_TMR_CALLBACK_PTR)timer1_callback,	//��ʱ��2�ص�����
                (void	    *)0,			
                (OS_ERR	    *)&err);

				
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
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OS_TaskSuspend((OS_TCB *)&StartTaskTCB,&err);
//	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}
void task1_task(void *p_arg)
{
	u8 num;
	OS_ERR err;
	p_arg = p_arg;
	
	while(1)
	{
		key = KEY_Scan();
		
		switch(key)
		{
			case KEY0_UP: 
				OSTmrStart(&timer0,&err);
				printf("������ʱ��1��\r\n");break;
			case KEY1_UP:
				OSTmrStart(&timer1,&err);
				printf("�����˶�ʱ��2!\r\n");break;
			case WK_UP:
				OSTmrStop(&timer0,OS_OPT_TMR_NONE,0,&err);
				OSTmrStop(&timer1,OS_OPT_TMR_NONE,0,&err);
				printf("�ر��˶�ʱ��1��2!!\r\n");break;
		}
		
		num++;
		if(num==50)
		{
			num = 0;
			LED0 = !LED0;
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
	}
}
void timer0_callback(void *p_tmr,void *p_arg)
{
	static u8 timer0_count = 0;
	
	LCD_ShowNum(62,111,timer0_count,3,16); //��ʾ��ʱ��1��ִ�д���
	LCD_Fill(6,131,114,313,lcd_color[timer0_count%14]);//�������
	timer0_count++;
}
void timer1_callback(void *p_tmr,void *p_arg)
{
	static u8 timer1_count = 0;
	timer1_count++;		//��ʱ��2ִ�д�����1
	LCD_ShowNum(182,111,timer1_count,3,16);  //��ʾ��ʱ��1ִ�д���
	LCD_Fill(126,131,233,313,lcd_color[timer1_count%14]); //�������
	LED1 = ~LED1;
	printf("��ʱ��2���н���\r\n");
}


