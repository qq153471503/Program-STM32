#include "display_fun.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

extern GUI_BITMAP LogoTable;  //ALIENTEK图标

//显示C文件格式的位图
void draw_bitmap(void)
{
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();
	
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);	//透明显示
	GUI_DispStringHCenterAt(" BITMAP DISPLAY TEST",320/2, 0);
	
	GUI_DrawBitmap(&LogoTable,0,0);  //绘制ALIENTEK图标
}

//在显示器中缩放位图
//Xmag:X方向的比例因子，单位为千分之
//Ymag:Y方向的比例因子，单位为千分之
void zoom_bitmap(int Xmag,int Ymag)
{
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_DrawBitmapEx(&LogoTable,400,240,105,46,Xmag,Ymag); //按照比例绘制位图
}
