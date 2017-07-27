/*>>>
	UCOSIII���������ȼ��û�������ʹ�ã�����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
	���ȼ�0���жϷ������������� OS_IntQTask()
	���ȼ�1��ʱ�ӽ������� OS_TickTask()
	���ȼ�2����ʱ���� OS_TmrTask()
	���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
	���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
<<<*/
	
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "ILI93XX.h"
#include "usart.h"	  
#include "sram.h"
#include "malloc.h"
#include "string.h"
#include "usmart.h"	
#include "GUI.H"
#include "includes.h"

void display1(void);
void display2(void);
void display3(void);
void display4(void);
void display5(void);
void display6(void);

//��ʼ����
#define START_TASK_PRIO		3	
#define START_STK_SIZE 		128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

//led0����
#define LED0_TASK_PRIO		4
#define LED0_STK_SIZE 		64
OS_TCB Led0TaskTCB;
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//led1����
#define LED1_TASK_PRIO		5
#define LED1_STK_SIZE 		64
OS_TCB Led1TaskTCB;
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
void led1_task(void *p_arg);

//lcd��ʾ����
#define LCD_TASK_PRIO	6
#define LCD_STK_SIZE	512
OS_TCB LCD_TAST_TCB;
CPU_STK LCD_TASK_STK[LCD_STK_SIZE];
void lcd_task(void *pdata);

 int main(void)
 {	 
	 
	 OS_ERR err;
	 CPU_SR_ALLOC();
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	TFTLCD_Init();			   		//��ʼ��LCD   
 	FSMC_SRAM_Init();			//��ʼ���ⲿSRAM  
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	
	OSInit(&err);		
	OS_CRITICAL_ENTER();

	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		
				 (CPU_CHAR	* )"start task", 		
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
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);
#endif		
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	GUI_Init();
	
	OS_CRITICAL_ENTER();

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

	OSTaskCreate((OS_TCB 	* )&LCD_TAST_TCB,		
				 (CPU_CHAR	* )"lcd task", 		
                 (OS_TASK_PTR )lcd_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LCD_TASK_PRIO,     	
                 (CPU_STK   * )&LCD_TASK_STK[0],	
                 (CPU_STK_SIZE)LCD_STK_SIZE/10,	
                 (CPU_STK_SIZE)LCD_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err); 
	OS_CRITICAL_EXIT();	
}

void led0_task(void *p_arg)
{
	
	OS_ERR err;
	p_arg = p_arg ;
	
	while(1)
	{
		LED0 = 1;
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_PERIODIC, &err);
		LED0 = 0;
		OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_PERIODIC, &err);
	}
}

void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg ;
	
	while(1)
	{
		LED1 = 0;
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_PERIODIC, &err);
		LED1 = 1;
		OSTimeDlyHMSM(0, 0, 0, 300, OS_OPT_TIME_PERIODIC, &err);
	}
}

void lcd_task(void *pdata)
{
	pdata = pdata;

  	while(1)
	{
		display1();
		GUI_Delay(1500);
		display2();
		GUI_Delay(1500);
		display3();
		GUI_Delay(1500);
		display4();
		GUI_Delay(1500);
		display5();
		GUI_Delay(1500);
	}
}

//2Dͼ�λ�������
void display1(void)
{
	int i=0;
	char *emwin_version;			//��������emwin�İ汾��
	GUI_SetBkColor(GUI_BLUE);		//���ñ���ɫΪ��ɫ
	GUI_Clear();					//����
	GUI_SetColor(GUI_YELLOW);		//����ǰ��ɫ
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringHCenterAt("2D display test ", 239/2, 0);
	emwin_version = (char *)GUI_GetVersionString();	//��ȡemwin�İ汾��
	GUI_DispStringAt("STemwin Version: ", 0, 319-24);
	GUI_DispString(emwin_version);
	
	GUI_SetColor(GUI_RED);		//����ǰ��ɫΪ��ɫ
	
	GUI_SetBkColor(GUI_GREEN);
	GUI_ClearRect(10,50,60,110);	//��һ���������������ɫ����
	GUI_SetBkColor(GUI_BLUE);		//�����Ļ���ɫ
	
	GUI_DrawGradientH(70, 50, 120, 110, 0x0000FF, 0x00FFFF);		//������ˮƽ��ɫ�ݶ����ľ���
	GUI_DrawGradientV(140, 50, 190, 110, 0x0000FF, 0x00FFFF);		//�����ô�ֱ��ɫ�ݶ����ľ���
	
	GUI_DrawGradientRoundedH(70, 130, 120, 190, 25, 0x0000FF, 0x00FFFF); 	//����ˮƽ��ɫ�ݶ�����Բ�Ǿ���
	GUI_DrawGradientRoundedV(140, 130, 190, 190 , 25, 0x0000FF, 0x00FFFF);	//���ƴ�ֱ��ɫ�ݶ�����Բ�Ǿ���

	GUI_DrawRect(10, 200, 60, 260);					//���ƾ���
	GUI_DrawRoundedRect(70, 200, 120, 260, 10);		//����Բ�Ǿ���
	GUI_FillRect(130, 200, 180, 260);				//�ڵ�ǰ������ָ��λ�û������ľ�������
	
	GUI_SetPenSize(5);				//���û�����ɫ,��λ���ص�
	GUI_DrawLine(0,35,239,35);  	//��������

	for(i=0; i<=30; i+=5)
	{
		GUI_DrawCircle((10+60)/2, (270+60)/2, i);		//����Բ��
	}
	GUI_FillCircle(215, 90, 22);						//��������Բ��
	
	GUI_SetPenSize(2);
	GUI_SetColor(GUI_CYAN);
	
	GUI_DrawEllipse(215,  (110-50)/2+150, 20, 30);		//������Բ
	GUI_FillEllipse(215, (110-50)/2+220, 20, 30);		//����������Բ
}
//Alpha ���
void display2(void)
{
	GUI_EnableAlpha(1);										//����Alpha���
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	GUI_SetColor(GUI_BLACK);
	GUI_DispStringHCenterAt("Alphablending", 239/2, 319/2-70);
	GUI_SetColor(0x40<<24 | GUI_RED);
	GUI_FillRect(10, 10, 110, 110);
	GUI_SetColor(0x80<<24 | GUI_GREEN);
	GUI_FillRect(110/2, 110/2, 110/2+100, 110/2+100);
	GUI_SetColor(0xc0<<24 | GUI_BLUE);
	GUI_FillRect((155/2+100)/2, (155/2+100)/2, 155/2+100, 155/2+100);
}
//���ƻ���
void display3(void)
{
	GUI_SetPenSize( 5 );
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringHCenterAt("GUI_DrawArc() Test",  239/2, 319/2);
	GUI_DrawArc(239/2, 319/2, 100, 100, -30, 210);
}

//�����Ǳ���
void display4(void)
{
	int x0 = 240/2;			//Բ�����ĵ�x����
	int y0 = 320/2;			//Բ�����ĵ�y����
	int r_x=110;			//Բ��x�뾶
	int r_y=110;			//Բ��y�뾶
	int cir_start = -30;	//Բ����ʼ�Ƕ�
	int cir_end = 210;		//Բ�������Ƕ�
	int i;
	char ac[4];
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
	GUI_SetPenSize(3);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontComic18B_ASCII);
	GUI_SetColor(GUI_YELLOW);
	GUI_DrawArc(x0, y0, r_x, r_y,cir_start, cir_end);
	GUI_DrawPoint(x0, y0);								//Բ��
	for (i=0; i<= 24; i++) 
	{
		float a = (cir_start+i*10)*3.1415926/180;
		int x = -(r_x-9)*cos(a)+x0;
		int y = -(r_y-9)*sin(a)+y0;
		
		if (i%2 == 0)
		  GUI_SetPenSize( 5 );
		else
		  GUI_SetPenSize( 4 );
		GUI_DrawPoint(x, y);
		
		if (i%2 == 0) 
		{
		  x = -(r_x-27)*cos(a) + x0;
		  y = -(r_y-20)*sin(a) + y0;
		  sprintf(ac, "%d", 10*i);
		  GUI_SetTextAlign(GUI_TA_VCENTER);
		  GUI_DispStringHCenterAt(ac,x,y);
		}
	}
}
	
//������ͼ
void display5(void)
{
	I16 aY[240];
	 int i;
//	int k=31;
//	char flag=0;
	
	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
	GUI_Clear();

//	for(i=0; i<GUI_COUNTOF(aY); i++)
//	{
//		aY[i] = k;

//		if(k >= 80)
//			flag = 1;
//		if(k <= 30)
//			flag = 0;
//		
//		if(flag)
//			k--;
//		else 
//			k++;
//	}
	
	for (i = 0; i < GUI_COUNTOF(aY); i++)
		aY[i] = rand()%100;

	GUI_ClearRect(0, 120, 240, 220);
	GUI_DrawGraph(aY, GUI_COUNTOF(aY), 0, 120);
}
