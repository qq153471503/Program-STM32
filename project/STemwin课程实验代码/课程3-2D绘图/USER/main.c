/*>>>
	UCOSIII中以下优先级用户程序不能使用，将这些优先级分配给了UCOSIII的5个系统内部任务
	优先级0：中断服务服务管理任务 OS_IntQTask()
	优先级1：时钟节拍任务 OS_TickTask()
	优先级2：定时任务 OS_TmrTask()
	优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
	优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
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

//开始任务
#define START_TASK_PRIO		3	
#define START_STK_SIZE 		128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

//led0任务
#define LED0_TASK_PRIO		4
#define LED0_STK_SIZE 		64
OS_TCB Led0TaskTCB;
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//led1任务
#define LED1_TASK_PRIO		5
#define LED1_STK_SIZE 		64
OS_TCB Led1TaskTCB;
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
void led1_task(void *p_arg);

//lcd显示任务
#define LCD_TASK_PRIO	6
#define LCD_STK_SIZE	512
OS_TCB LCD_TAST_TCB;
CPU_STK LCD_TASK_STK[LCD_STK_SIZE];
void lcd_task(void *pdata);

 int main(void)
 {	 
	 
	 OS_ERR err;
	 CPU_SR_ALLOC();
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	TFTLCD_Init();			   		//初始化LCD   
 	FSMC_SRAM_Init();			//初始化外部SRAM  
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
	
	OSInit(&err);		
	OS_CRITICAL_ENTER();

	//创建开始任务
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
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
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

//2D图形绘制例程
void display1(void)
{
	int i=0;
	char *emwin_version;			//用来储存emwin的版本号
	GUI_SetBkColor(GUI_BLUE);		//设置背景色为蓝色
	GUI_Clear();					//清屏
	GUI_SetColor(GUI_YELLOW);		//设置前景色
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringHCenterAt("2D display test ", 239/2, 0);
	emwin_version = (char *)GUI_GetVersionString();	//获取emwin的版本号
	GUI_DispStringAt("STemwin Version: ", 0, 319-24);
	GUI_DispString(emwin_version);
	
	GUI_SetColor(GUI_RED);		//设置前景色为红色
	
	GUI_SetBkColor(GUI_GREEN);
	GUI_ClearRect(10,50,60,110);	//在一个矩形区域填充绿色背景
	GUI_SetBkColor(GUI_BLUE);		//背景改回蓝色
	
	GUI_DrawGradientH(70, 50, 120, 110, 0x0000FF, 0x00FFFF);		//绘制用水平颜色梯度填充的矩形
	GUI_DrawGradientV(140, 50, 190, 110, 0x0000FF, 0x00FFFF);		//绘制用垂直颜色梯度填充的矩形
	
	GUI_DrawGradientRoundedH(70, 130, 120, 190, 25, 0x0000FF, 0x00FFFF); 	//绘制水平颜色梯度填充的圆角矩形
	GUI_DrawGradientRoundedV(140, 130, 190, 190 , 25, 0x0000FF, 0x00FFFF);	//绘制垂直颜色梯度填充的圆角矩形

	GUI_DrawRect(10, 200, 60, 260);					//绘制矩形
	GUI_DrawRoundedRect(70, 200, 120, 260, 10);		//绘制圆角矩形
	GUI_FillRect(130, 200, 180, 260);				//在当前窗口中指定位置绘制填充的矩形区域
	
	GUI_SetPenSize(5);				//设置画笔颜色,单位像素点
	GUI_DrawLine(0,35,239,35);  	//绘制线条

	for(i=0; i<=30; i+=5)
	{
		GUI_DrawCircle((10+60)/2, (270+60)/2, i);		//绘制圆形
	}
	GUI_FillCircle(215, 90, 22);						//绘制填充的圆形
	
	GUI_SetPenSize(2);
	GUI_SetColor(GUI_CYAN);
	
	GUI_DrawEllipse(215,  (110-50)/2+150, 20, 30);		//绘制椭圆
	GUI_FillEllipse(215, (110-50)/2+220, 20, 30);		//绘制填充的椭圆
}
//Alpha 混合
void display2(void)
{
	GUI_EnableAlpha(1);										//启用Alpha混合
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
//绘制弧线
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

//绘制仪表盘
void display4(void)
{
	int x0 = 240/2;			//圆弧中心的x坐标
	int y0 = 320/2;			//圆弧中心的y坐标
	int r_x=110;			//圆弧x半径
	int r_y=110;			//圆弧y半径
	int cir_start = -30;	//圆弧开始角度
	int cir_end = 210;		//圆弧结束角度
	int i;
	char ac[4];
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
	GUI_SetPenSize(3);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontComic18B_ASCII);
	GUI_SetColor(GUI_YELLOW);
	GUI_DrawArc(x0, y0, r_x, r_y,cir_start, cir_end);
	GUI_DrawPoint(x0, y0);								//圆心
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
	
//绘制线图
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
