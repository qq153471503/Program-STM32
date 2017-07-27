
#include "DIALOG.h"
#include "stdlib.h"
#include "bitmap.h"
#include "stdio.h"
#include "adc.h"
#include "led.h"

#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x06)
#define ID_GRAPH_0 (GUI_ID_USER + 0x07)
#define ID_IMAGE_0 (GUI_ID_USER + 0x08)
#define ID_TEXT_0 (GUI_ID_USER + 0x09)
#define ID_TEXT_1 (GUI_ID_USER + 0x0A)
#define ID_TEXT_2 (GUI_ID_USER + 0x0B)
#define ID_IMAGE_1 (GUI_ID_USER + 0x0C)
#define ID_IMAGE_2 (GUI_ID_USER + 0x0D)

#define ID_IMAGE_XIN 0x00
#define ID_IMAGE_XIAO 0x01
#define ID_IMAGE_KU 0x02

#define ID_EDIT_0 (GUI_ID_USER + 0x0F)
#define ID_EDIT_1 (GUI_ID_USER + 0x10)
#define ID_EDIT_2 (GUI_ID_USER + 0x11)

GRAPH_DATA_Handle  data_tab;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240, 0, 0x64, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 0, 0, 310, 140, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 1, 141, 75, 75, 0, IMAGE_CF_AUTOSIZE, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 79, 143, 62, 18, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 79, 193, 62, 18, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_2, 75, 168, 72, 19, 0, 0x64, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_1, 234, 141, 75, 75, 0, IMAGE_CF_AUTOSIZE, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_2, 160, 141, 75, 75, 0, IMAGE_CF_AUTOSIZE, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 150, 143, 65, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_1, 150, 169, 65, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_2, 150, 194, 65, 20, 0, 0x64, 0 },
};

/*********************************************************************
*
*       _GetImageById
*/
static const void * _GetImageById(U32 Id, U32 * pSize) {
  switch (Id) {
  case ID_IMAGE_XIN:
    *pSize = sizeof(xin_tab);
    return (const void *)xin_tab;
  }
  return NULL;
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
	const void * pData;
	WM_HWIN      hItem;
	U32          FileSize;
	char          pData_edit[10];
	uint16_t runningTotal=0;
	uint8_t i;
	uint16_t Num;
  switch (pMsg->MsgId) {

  case WM_TIMER:{
        WM_RestartTimer(pMsg->Data.v, 2);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);

/*******************************************************************************/
	  	//读取到的值右移2位，12位-->10位
		//Signal = ADC_GetConversionValue(ADC1)/4;     // read the Pulse Senso
	  Signal = Get_Adc_Average(ADC_Channel_0, 10)>>2;
	  printf("%d\n",Signal);
		sampleCounter += 2;                          // keep track of the time in mS with this variable
		Num = sampleCounter - lastBeatTime;          // monitor the time since the last beat to avoid noise	
	
		//发现脉冲波的波峰和波谷
		//  find the peak and trough of the pulse wave
		if(Signal < thresh && Num > (IBI/5)*3){   // avoid dichrotic noise by waiting 3/5 of last IBI
			if (Signal < T){                        // T is the trough
				T = Signal;                           // keep track of lowest point in pulse wave 
			}
		}

		if(Signal > thresh && Signal > P){        // thresh condition helps avoid noise
			P = Signal;                             // P is the peak
		}                                         // keep track of highest point in pulse wave

	//开始寻找心跳
	//当脉冲来临的时候，signal的值会上升
	//  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
	// signal surges up in value every time there is a pulse
  if (Num > 250){                                 // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (Num > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
      LED0=0; 
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(i=0; i<=9; i++){                 // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                     //rate[0]-rate[9]的值都一样吗？？？？
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        return;                              // IBI value is unreliable so discard it
      }   

      // keep a running total of the last 10 IBI values
     // runningTotal = 0;                  		// clear the runningTotal variable    

      for(i=0; i<=8; i++){                    // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

	//脉冲开始下降
  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
//    LED0=1; 															 //灯灭
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  //没有检测到脉冲，设置默认值
	if (Num > 2500){                         // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
  }
/******************************************************************************************/
		GRAPH_DATA_YT_AddValue(data_tab,Signal-450);
        sprintf(pData_edit, "%d", Signal);
        EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), pData_edit);

		if(QS == true)
		{
			sprintf(pData_edit, "%d", BPM);
			EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1), pData_edit);
			sprintf(pData_edit, "%d", IBI);
			EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2), pData_edit);
			QS = false;
			
			if(BPM>60 && BPM<100)
				IMAGE_SetBitmap(hItem, &bmxiao);
			else 
				IMAGE_SetBitmap(hItem, &bmku);
		}

  }break;

  case WM_PAINT:{
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
  }break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Framewin'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_1);
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    FRAMEWIN_SetText(hItem, "Pulse Sensor Amped Interface");
    FRAMEWIN_SetTextColor(hItem, 0x00400080);
    //
    // Initialization of 'Graph'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
    GRAPH_SetBorder(hItem, 2, 2, 2, 2);
    data_tab = GRAPH_DATA_YT_Create(GUI_GREEN, 500, 0, 0);//????????,???????
    GRAPH_AttachData(hItem, data_tab);     //???????????
    //
    // Initialization of 'Image' xin
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
    pData = _GetImageById(ID_IMAGE_XIN, &FileSize);
    IMAGE_SetGIF(hItem, pData, FileSize);
    //
    // Initialization of 'Image' xiao
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);
    IMAGE_SetBitmap(hItem, &bmku);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetText(hItem, "*  Signal :");
    TEXT_SetFont(hItem, GUI_FONT_16B_1);
    TEXT_SetTextColor(hItem, 0x0000FFFF);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetText(hItem, "*      IBI   :");
    TEXT_SetFont(hItem, GUI_FONT_16B_1);
    TEXT_SetTextColor(hItem, 0x0000FFFF);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetText(hItem, "*    BPM   :");
    TEXT_SetFont(hItem, GUI_FONT_16B_1);
    TEXT_SetTextColor(hItem, 0x0000FFFF);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetText(hItem, "000");
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, 0x00FF0000);
    EDIT_SetFont(hItem, GUI_FONT_13_1);
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
    EDIT_SetText(hItem, "000");
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, 0x00FF0000);
    EDIT_SetFont(hItem, GUI_FONT_13_1);
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
    EDIT_SetText(hItem, "000");
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, 0x00FF0000);
    EDIT_SetFont(hItem, GUI_FONT_13_1);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       CreateFramewin
*/
WM_HWIN CreateFramewin_bs(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  WM_CreateTimer(WM_GetClientWindow(hWin),0, 500, 0); //
  return hWin;
}

void _cbBkWindow_bs(WM_MESSAGE *pMsg){
    switch(pMsg->MsgId){
        case WM_PAINT:{
            GUI_SetBkColor(GUI_WHITE);
            GUI_Clear();
        }break;
        default:
            WM_DefaultProc(pMsg);
    }
}
void mainTask(void){
    GUI_Init();
    WM_SetCallback(WM_HBKWIN, &_cbBkWindow_bs);
    WM_SetCreateFlags(WM_CF_MEMDEV);
//
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);

	CreateFramewin_bs();

	while(1)
    {
        GUI_Exec();
		GUI_Delay(10);
    }
}
/*************************** End of file ****************************/
