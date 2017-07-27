#include "display_fun.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

extern GUI_BITMAP LogoTable;  //ALIENTEKͼ��

//��ʾC�ļ���ʽ��λͼ
void draw_bitmap(void)
{
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();
	
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);	//͸����ʾ
	GUI_DispStringHCenterAt(" BITMAP DISPLAY TEST",320/2, 0);
	
	GUI_DrawBitmap(&LogoTable,0,0);  //����ALIENTEKͼ��
}

//����ʾ��������λͼ
//Xmag:X����ı������ӣ���λΪǧ��֮
//Ymag:Y����ı������ӣ���λΪǧ��֮
void zoom_bitmap(int Xmag,int Ymag)
{
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_DrawBitmapEx(&LogoTable,400,240,105,46,Xmag,Ymag); //���ձ�������λͼ
}
