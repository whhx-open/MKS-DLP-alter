/*******************************************************************************
*
*     Base user interface for LCD with touch
*
********************************************************************************/

#include "main.h"
#include "tgui_printscreenfuncs.h"
#include "tgui_defaultfuncs.h"
#include "printing.h"
#include "motor.h"
#include "z_endstops.h"
#include "sys_timer.h"


extern TG_SCREEN		*tguiActiveScreen;
extern char				msg[512];
extern __no_init FIL	ufile @ "CCMRAM";

extern char				fv_filename[256];
extern char				fv_dirname[256];
extern FILES_TYPE		fv_filetype;
extern TCHAR			fv_tfilename[512];

extern PRINT_STATE		prtState;

uint32_t				old_time = 0;
uint32_t				old_layer = 0;
uint8_t					old_pause = 0;


void		TGUI_PrintScreenShow(void *tguiobj, void *param)
{
	if (PRINT_Init() == 0)
	{
		TGUI_MessageBoxOk(LANG_GetString(LSTR_ERROR), LANG_GetString(LSTR_MSG_FILE_OPEN_ERROR));
		return;
	}

	tguiScreenPrint.prevscreen = tguiScreenFileview.prevscreen;
	tguiActiveScreen = &tguiScreenPrint;

	systemInfo.is_paused = 0;
	systemInfo.is_printing = 0;
	systemInfo.is_printhoming = 1;

	old_time = DTIME_GetCurrentUnixtime();
	old_layer = 0;

	for (uint8_t i = 0; i < tguiScreenPrint.btns_count; i++)
	{
		if (tguiScreenPrint.buttons[i].button_id == TG_SCR_PRINT_PAUSE_BTN_ID)
		{
			tguiScreenPrint.buttons[i].options.active = 0;
			break;
		}
	}

	ZMOTOR_StartHoming();

	TGUI_ForceRepaint();
}
//==============================================================================




void		TGUI_PrintScreenExit(void *tguiobj, void *param)
{
	systemInfo.is_paused = 0;
	systemInfo.is_printing = 0;
	systemInfo.is_printhoming = 0;
	FAN_LED_Off();

	tguiActiveScreen = (TG_SCREEN*)tguiScreenFileview.prevscreen;
	TGUI_ForceRepaint();
}
//==============================================================================




void		_tgui_PrintScreenProcess(void *tguiobj, void *param)
{
	if (old_time != DTIME_GetCurrentUnixtime() || old_layer != prtState.current_layer || old_pause != systemInfo.is_paused)
	{
		for (uint8_t i = 0; i < tguiScreenPrint.btns_count; i++)
		{
			if (tguiScreenPrint.buttons[i].button_id == TG_SCR_PRINT_INFO_ID)
			{
				_tgui_PrintScreenProgressUpdate((void*)&(tguiScreenPrint.buttons[i]), NULL);
				break;
			}
		}
		old_time = DTIME_GetCurrentUnixtime();
		old_layer = prtState.current_layer;
	}
	_tgui_DefaultScreenProcess(tguiobj, param);
}
//==============================================================================




void		_tgui_PrintScreenPausePress(void *tguiobj, void *param)
{
	if (systemInfo.is_paused)
		systemInfo.is_paused = 0;
	else
		systemInfo.is_paused = 1;
	for (uint8_t i = 0; i < tguiScreenPrint.btns_count; i++)
	{
		if (tguiScreenPrint.buttons[i].button_id == TG_SCR_PRINT_PAUSE_BTN_ID)
		{
			tguiScreenPrint.buttons[i].options.active = systemInfo.is_paused;
			tguiScreenPrint.buttons[i].options.needrepaint = 1;
			break;
		}
	}
	for (uint8_t i = 0; i < tguiScreenPrint.btns_count; i++)
	{
		if (tguiScreenPrint.buttons[i].button_id == TG_SCR_PRINT_INFO_ID)
		{
			_tgui_PrintScreenProgressUpdate((void*)&(tguiScreenPrint.buttons[i]), NULL);
			break;
		}
	}
	old_pause = systemInfo.is_paused;
}
//==============================================================================




void		_tgui_PrintScreenProgressPaint(void *tguiobj, void *param)
{
	TG_BUTTON		*thisbtn = (TG_BUTTON*)tguiobj;

	LCDUI_FONT_TYPE oldfont = LCDUI_SetFont(LCDUI_FONT_H18);
	uint16_t 		oldcolor = LCDUI_SetColor(LCDUI_RGB(0x00496C));
	uint16_t 		oldbackcolor = LCDUI_SetBackColor(LCDUI_RGB(0xDDDDDD));

	// file name
	LCDUI_SetColor(LCDUI_RGB(0x00496C));
	LCDUI_DrawText(fv_filename, LCDUI_TEXT_ALIGN_CENTER, thisbtn->position.left, thisbtn->position.top + 3, thisbtn->position.right, thisbtn->position.top + 22);


	// printing time
	LCDUI_SetFont(LCDUI_FONT_H18BOLD);
	LCDUI_SetColor(LCDUI_RGB(0x000000));
	LCDUI_DrawText(LANG_GetString(LSTR_TIME), 0, thisbtn->position.left + 5, thisbtn->position.top + 28, thisbtn->position.right - 366, -1);
	LCDUI_SetFont(LCDUI_FONT_H18);
	LCDUI_SetColor(LCDUI_RGB(0x00496C));
	uint32_t	est_time = PFILE_GetPrintTime();
	uint32_t	pass_time = DTIME_GetCurrentUnixtime() - prtState.time_begin;
	uint32_t	est_h = est_time / 3600;
	uint32_t	est_m = (est_time - (est_h * 3600)) / 60;
	uint32_t	pass_h = pass_time / 3600;
	uint32_t	pass_m = (pass_time - (pass_h * 3600)) / 60;
	sprintf(msg, (char*)"%02u:%02u:%02u/%02u:%02u:%02u", pass_h, pass_m, pass_time % 60, est_h, est_m, est_time % 60);
	LCDUI_DrawText(msg, LCDUI_TEXT_ALIGN_RIGHT, thisbtn->position.left + 100, thisbtn->position.top + 28, thisbtn->position.right - 196, -1);
	
	// layers
	LCDUI_SetFont(LCDUI_FONT_H18BOLD);
	LCDUI_SetColor(LCDUI_RGB(0x000000));
	LCDUI_DrawText(LANG_GetString(LSTR_LAYERS), 0, thisbtn->position.left + 5, thisbtn->position.top + 48, thisbtn->position.right - 366, -1);
	LCDUI_SetFont(LCDUI_FONT_H18);
	LCDUI_SetColor(LCDUI_RGB(0x00496C));
	sprintf(msg, (char*)"%u/%u", prtState.current_layer + 1, PFILE_GetTotalLayers());
	LCDUI_DrawText(msg, LCDUI_TEXT_ALIGN_RIGHT, thisbtn->position.left + 100, thisbtn->position.top + 48, thisbtn->position.right - 196, -1);

	// progress bar
	LCDUI_DrawRect(thisbtn->position.left + 5, thisbtn->position.top + 69, 380, 18);
	float		f_proc = ((float)(prtState.current_layer + 1) / (float)PFILE_GetTotalLayers()) * 100;
	uint32_t	proc = (uint32_t)f_proc;
	f_proc = (376.0 / 100.0) * f_proc;
	uint32_t		pb_width = (uint32_t)f_proc;
	LCDUI_FillRect(thisbtn->position.left + 7, thisbtn->position.top + 71, pb_width, 14);
	// percents
	LCDUI_SetFont(LCDUI_FONT_H18BOLD);
	sprintf(msg, (char*)"%u%%", proc);
	LCDUI_DrawText(msg, LCDUI_TEXT_ALIGN_RIGHT, thisbtn->position.left + 387, thisbtn->position.top + 70, thisbtn->position.right - 10, -1);

	// state (printing/pause)
	LCDUI_SetFont(LCDUI_FONT_H24BOLD);
	if (systemInfo.is_paused)
	{
		LCDUI_SetColor(LCDUI_RGB(0xA67500));
		LCDUI_DrawText(LANG_GetString(LSTR_PAUSE_CAPS), LCDUI_TEXT_ALIGN_CENTER, thisbtn->position.left + 272, thisbtn->position.top + 37, thisbtn->position.right - 5, -1);
	}
	else
	{
		LCDUI_SetColor(LCDUI_RGB(0x009700));
		LCDUI_DrawText(LANG_GetString(LSTR_PRINTING_CAPS), LCDUI_TEXT_ALIGN_CENTER, thisbtn->position.left + 272, thisbtn->position.top + 37, thisbtn->position.right - 5, -1);
	}

	LCDUI_SetColor(oldcolor);
	LCDUI_SetBackColor(oldbackcolor);
	LCDUI_SetFont(oldfont);
}
//==============================================================================




void		_tgui_PrintScreenProgressUpdate(void *tguiobj, void *param)
{
	TG_BUTTON		*thisbtn = (TG_BUTTON*)tguiobj;

	LCDUI_FONT_TYPE oldfont = LCDUI_SetFont(LCDUI_FONT_H18);
	uint16_t 		oldcolor = LCDUI_SetColor(LCDUI_RGB(0x00496C));
	uint16_t 		oldbackcolor = LCDUI_SetBackColor(LCDUI_RGB(0xDDDDDD));

	// clear
	
	// printing time
	if (old_time != DTIME_GetCurrentUnixtime())
	{
		uint32_t	est_time = PFILE_GetPrintTime();
		uint32_t	pass_time = DTIME_GetCurrentUnixtime() - prtState.time_begin;
		uint32_t	est_h = est_time / 3600;
		uint32_t	est_m = (est_time - (est_h * 3600)) / 60;
		uint32_t	pass_h = pass_time / 3600;
		uint32_t	pass_m = (pass_time - (pass_h * 3600)) / 60;
		sprintf(msg, (char*)"%02u:%02u:%02u/%02u:%02u:%02u", pass_h, pass_m, pass_time % 60, est_h, est_m, est_time % 60);
		LCDUI_SetColor(LCDUI_RGB(0xDDDDDD));
		LCDUI_FillRect(thisbtn->position.left + 86, thisbtn->position.top + 28, 190, 18);
		LCDUI_SetColor(LCDUI_RGB(0x00496C));
		LCDUI_DrawText(msg, LCDUI_TEXT_ALIGN_RIGHT, thisbtn->position.left + 100, thisbtn->position.top + 28, thisbtn->position.right - 196, -1);
	}
	
	// layers
	if (old_layer != prtState.current_layer)
	{
		LCDUI_SetColor(LCDUI_RGB(0x00496C));
		sprintf(msg, (char*)"%u/%u", prtState.current_layer + 1, PFILE_GetTotalLayers());
		LCDUI_SetColor(LCDUI_RGB(0xDDDDDD));
		LCDUI_FillRect(thisbtn->position.left + 86, thisbtn->position.top + 48, 190, 18);
		LCDUI_FillRect(thisbtn->position.left + 386, thisbtn->position.top + 70, 72, 18);

		LCDUI_SetColor(LCDUI_RGB(0x00496C));
		LCDUI_DrawText(msg, LCDUI_TEXT_ALIGN_RIGHT, thisbtn->position.left + 100, thisbtn->position.top + 48, thisbtn->position.right - 196, -1);

		// progress bar
		float		f_proc = ((float)(prtState.current_layer + 1) / (float)PFILE_GetTotalLayers()) * 100;
		uint32_t	proc = (uint32_t)f_proc;
		f_proc = (376.0 / 100.0) * f_proc;
		uint32_t		pb_width = (uint32_t)f_proc;
		LCDUI_FillRect(thisbtn->position.left + 7, thisbtn->position.top + 71, pb_width, 14);
		// percents
		LCDUI_SetFont(LCDUI_FONT_H18BOLD);
		sprintf(msg, (char*)"%u%%", proc);
		LCDUI_DrawText(msg, LCDUI_TEXT_ALIGN_RIGHT, thisbtn->position.left + 387, thisbtn->position.top + 70, thisbtn->position.right - 10, -1);
	
	}

	// state (printing/pause)
	if (old_pause != systemInfo.is_paused)
	{
		LCDUI_SetColor(LCDUI_RGB(0xDDDDDD));
		LCDUI_FillRect(thisbtn->position.left + 270, thisbtn->position.top + 35, 195, 25);
		LCDUI_SetFont(LCDUI_FONT_H24BOLD);
		if (systemInfo.is_paused)
		{
			LCDUI_SetColor(LCDUI_RGB(0xA67500));
			LCDUI_DrawText(LANG_GetString(LSTR_PAUSE_CAPS), LCDUI_TEXT_ALIGN_CENTER, thisbtn->position.left + 272, thisbtn->position.top + 37, thisbtn->position.right - 5, -1);
		}
		else
		{
			LCDUI_SetColor(LCDUI_RGB(0x009700));
			LCDUI_DrawText(LANG_GetString(LSTR_PRINTING_CAPS), LCDUI_TEXT_ALIGN_CENTER, thisbtn->position.left + 272, thisbtn->position.top + 37, thisbtn->position.right - 5, -1);
		}
	}

	LCDUI_SetColor(oldcolor);
	LCDUI_SetBackColor(oldbackcolor);
	LCDUI_SetFont(oldfont);
}
//==============================================================================



