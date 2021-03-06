#include "touch.h"
#include "fst_spi.h"
#include "lcd_low.h"
#include "sys_timer.h"
#include "config.h"




#define TOUCH_PRESSED()		(touch_point.xc || touch_point.yc)

extern uint8_t				tguiScreenTimer;

extern uint8_t				touch_buff[TOUCH_BUFF_SIZE];
TOUCH_INFO					touch_info;
TOUCH_POINT					touch_point;





void		_touch_ReadCoords()
{
	if (TOUCH_SPIGetState() != HAL_SPI_STATE_READY)
	{
		touch_point.xc = touch_point.yc = 0;
		return;
	}
	
	uint32_t	vavg = 0;
	uint8_t		*buff = touch_buff+1;

	// Calculate X coord
	for (uint8_t i = 0; i < TOUCH_READS; i++)
	{
		vavg += (uint16_t)(*buff)<<8;
		buff++;
		vavg += *buff;
		buff += 5;
	}
	vavg = vavg / TOUCH_READS;
	if (vavg < touch_info.x_min || vavg > touch_info.x_max)
	{
		touch_point.xc = 0;
	}
	else
	{
		if (cfgConfig.display_rotate == 0)
			touch_point.xc = LCD_WIDTH - (vavg - touch_info.x_min) * LCD_WIDTH / (touch_info.x_max - touch_info.x_min);
		else
			touch_point.xc = (vavg - touch_info.x_min) * LCD_WIDTH / (touch_info.x_max - touch_info.x_min);
	}

	// Calculate Y coord
	vavg = 0;
	buff = touch_buff+4;

	for (uint8_t i = 0; i < TOUCH_READS; i++)
	{
		vavg += (uint16_t)(*buff)<<8;
		buff++;
		vavg += *buff;
		buff += 5;
	}
	vavg = vavg / TOUCH_READS;
	if (vavg < touch_info.y_min || vavg > touch_info.y_max)
	{
		touch_point.yc = 0;
	}
	else
	{
		if (cfgConfig.display_rotate == 0)
			touch_point.yc = (vavg - touch_info.y_min) * LCD_HEIGHT / (touch_info.y_max - touch_info.y_min);
		else
			touch_point.yc = LCD_HEIGHT - (vavg - touch_info.y_min) * LCD_HEIGHT / (touch_info.y_max - touch_info.y_min);
	}
	
	
	// Start new read
	TOUCH_SPIStartRead();
	
	_touch_RefreshState();
	return;
}
//==============================================================================




void		_touch_RefreshState()
{
	switch (touch_info.state)
	{
		case TS_WORKED:
			switch (touch_info.prevstate)
			{
				case TS_SRELEASED:
				case TS_LRELEASED:
					if (TOUCH_PRESSED())
					{
						touch_info.state = TS_PREPRESSED;
						touch_info.time = 0;
					}
					else
						touch_info.state = TS_FREE;
					break;
				case TS_SPRESSED:
					if (TOUCH_PRESSED())
					{
						if (touch_info.time > 100)
						{
							touch_info.state = TS_LPRESSED;
						}
						else
							touch_info.time++;
					}
					else
					{
						touch_info.state = TS_SRELEASED;
						touch_info.time = 0;
					}
					break;
				case TS_LPRESSED:
					if (!TOUCH_PRESSED())
					{
						touch_info.state = TS_LRELEASED;
						touch_info.time = 0;
					}
					break;
				default:
					touch_info.state = TS_FREE;
					break;
			}
			break;
		case TS_FREE:
		case TS_SRELEASED:
		case TS_LRELEASED:
			if (TOUCH_PRESSED())
			{
				touch_info.state = TS_PREPRESSED;
				touch_info.time = 0;
			}
			break;
		case TS_PREPRESSED:
			if (TOUCH_PRESSED())
			{
				if (touch_info.time > 1)
				{
					touch_info.state = TS_SPRESSED;
					SYSTIMER_SetCountDown(tguiScreenTimer, cfgConfig.screensaver_time);
				}
				else
					touch_info.time++;
			}
			else
			{
				touch_info.state = TS_WORKED;
				touch_info.time = 0;
			}
			break;
		case TS_SPRESSED:
			if (TOUCH_PRESSED())
			{
				if (touch_info.time > 50)
				{
					touch_info.state = TS_LPRESSED;
				}
				else
					touch_info.time++;
			}
			else
			{
				touch_info.state = TS_SRELEASED;
				touch_info.time = 0;
			}
			break;
		case TS_LPRESSED:
			if (!TOUCH_PRESSED())
			{
				touch_info.state = TS_LRELEASED;
				touch_info.time = 0;
			}
			else
				touch_info.time++;
			break;
	}
}
//==============================================================================












void		Touch_Init(void)
{
	TOUCH_SPIInit();
	
	touch_info.x_min = 1500;
	touch_info.x_max = 31300;
	touch_info.y_min = 1000;
	touch_info.y_max = 31200;
	touch_info.state = TS_FREE;
	touch_info.time = 0;

	touch_point.xc = 0;
	touch_point.yc = 0;
}
//==============================================================================




void		Touch_Enable()
{
	TOUCH_SPIEnable();
}
//==============================================================================




void		Touch_Disable()
{
	TOUCH_SPIDisable();
}
//==============================================================================




TOUCH_STATES	Touch_GetState()
{
	return touch_info.state;
}
//==============================================================================




void		Touch_SetWorked(TOUCH_STATES state)
{
	// check current state for protect again delayed change
	if (touch_info.state == state)
	{
		touch_info.prevstate = touch_info.state;
		touch_info.state = TS_WORKED;
	}
}
//==============================================================================




void		Touch_GetCoords(TOUCH_POINT *pt)
{
	pt->xc = touch_point.xc;
	pt->yc = touch_point.yc;
}
//==============================================================================




uint8_t		Touch_IsPressed()
{
	return touch_point.xc || touch_point.yc;
}
//==============================================================================




