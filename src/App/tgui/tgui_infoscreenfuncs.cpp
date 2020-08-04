/*******************************************************************************
*
*     Base user interface for LCD with touch
*
********************************************************************************/

#include "tgui_infoscreenfuncs.h"
#include "tgui_defaultfuncs.h"




	
void		_tgui_InfoScreenVersionPaint(void *tguiobj, void *param)
{
	TG_BUTTON		*thisbtn = (TG_BUTTON*)tguiobj;
	
	char	msg[32];
	sprintf(msg, "v%u.%02u ", (FW_VERSION >> 8) & 0xFF, FW_VERSION & 0xFF);
	thisbtn->text = (LNG_STRING_ID)((DWORD)&msg);
	_tgui_DefaultButtonPaint(tguiobj, NULL);
	thisbtn->text = LNGS____;
}
//==============================================================================




void		_tgui_InfoScreenLightTimePaint(void *tguiobj, void *param)
{
	TG_BUTTON		*thisbtn = (TG_BUTTON*)tguiobj;
	
	char	msg[32];
	sprintf(msg, "%u ", 18);
	strcat(msg, LANG_GetString(LNGS_SHORTHOUR));
	thisbtn->text = (LNG_STRING_ID)((DWORD)&msg);
	_tgui_DefaultButtonPaint(tguiobj, NULL);
	thisbtn->text = LNGS____;
}
//==============================================================================




void		_tgui_InfoScreenFansTimePaint(void *tguiobj, void *param)
{
	TG_BUTTON		*thisbtn = (TG_BUTTON*)tguiobj;
	
	char	msg[32];
	sprintf(msg, "%u ", 21);
	strcat(msg, LANG_GetString(LNGS_SHORTHOUR));
	thisbtn->text = (LNG_STRING_ID)((DWORD)&msg);
	_tgui_DefaultButtonPaint(tguiobj, NULL);
	thisbtn->text = LNGS____;
}
//==============================================================================




