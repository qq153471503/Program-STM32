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

void _Draw(int Delay);
static GUI_RECT Rect = {0, 130, 100, 180};
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


void _Draw(int Delay) 
{
	  GUI_SetPenSize(5);
	  GUI_SetColor(GUI_RED);
	  GUI_DrawLine(Rect.x0 + 3, Rect.y0 + 3, Rect.x1 - 3, Rect.y1 - 3);
	  GUI_Delay(Delay);
	  GUI_SetColor(GUI_GREEN);
	  GUI_DrawLine(Rect.x0 + 3, Rect.y1 - 3, Rect.x1 - 3, Rect.y0 + 3);
	  GUI_Delay(Delay);
	  GUI_SetColor(GUI_WHITE);
	  GUI_SetFont(&GUI_FontComic24B_ASCII);
	  GUI_SetTextMode(GUI_TM_TRANS);
	  GUI_DispStringInRect("Closed", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
	  GUI_Delay(Delay);
}

void lcd_task(void *pdata) {
	
  GUI_MEMDEV_Handle hMem;
  int i;

  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font24_ASCII);
  GUI_DispStringHCenterAt("MEMDEV_MemDev - Sample", 160, 5);
  GUI_SetFont(&GUI_Font8x16);
  GUI_DispStringHCenterAt("Shows the advantage of using a\nmemorydevice", 160,50);
  GUI_SetFont(&GUI_Font8_1);
  GUI_DispStringHCenterAt("Draws the picture\nwithout a\nmemory device", 50, 90);
  GUI_DispStringHCenterAt("Draws the picture\nusing a\nmemory device", 270, 90);

	//���������豸�������豸���
  hMem = GUI_MEMDEV_Create(Rect.x0, Rect.y0, Rect.x1 - Rect.x0, Rect.y1 - Rect.y0);

	//ѡ���豸
  GUI_MEMDEV_Select(hMem);
	
	//���ڴ��л滭
  _Draw(0);
  
   //����洢�豸 ���Ϊ0�򼤻�LCD
  GUI_MEMDEV_Select(0);
  
	while (1) 
	{
		for (i = 0; i < 3; i++) 
		{
		  GUI_Delay(250);
		  GUI_ClearRect(LCD_GetXSize() - Rect.x1, Rect.y0, LCD_GetXSize(), Rect.y1);
		  GUI_Delay(250);
		  GUI_MEMDEV_CopyToLCDAt(hMem, LCD_GetXSize() - Rect.x1, Rect.y0);
		}
		GUI_Delay(500);  
		
		//δʹ�ô洢�豸
		_Draw(400);
		GUI_Delay(400);
		GUI_ClearRect(0, 130, 319, 219);
	}
}
