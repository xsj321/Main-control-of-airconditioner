#ifndef __SYSTEMUI_H__
#define __SYSTEMUI_H__
#define TOUCH_DELAY 1000000
#define TOUCH_MINUS_BUTTON_RIGHT 1300
#define TOUCH_MINUS_BUTTON_LIFT 1700
#define TOUCH_PLUS_BUTTON_RIGHT 100
#define TOUCH_PLUS_BUTTON_LIFT 600
#define DISPLAY_INFOBOX_LIFT 105
void Int_To_Str(int x,char *Str);
void UI_Refresh(int INT_TEM,int INT_HIM,int INT_NOWTEM);
void UI_Value_Refresh(int INT_TEM,int INT_HIM,int INT_NOWTEM);
int Str_to_int(unsigned char *source);
#endif
