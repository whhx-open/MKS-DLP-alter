#ifndef __config_H
#define __config_H

#include "includes.h"
#include "main.h"
#include "ff.h"






#define			EEPR_ADDR_MOTORCONFIG	0x0000
#define			EEPR_ADDR_GLOBALCONFIG	0x0100
#define			EEPR_ADDR_WORKTIME		0x0200






typedef enum
{
	PST_IDLE = 0,
	PST_HOMING_PREUP,
	PST_HOMING_FAST,
	PST_HOMING_UP,
	PST_HOMING_SLOW,
	PST_FREEMOVING_UP,
	PST_FREEMOVING_DOWN,
	
	PST_PRINT_MOVETOLAYER,
	PST_PRINT_LIGHTPAUSE,
	PST_PRINT_LIGHT,
	PST_PRINT_LIFT,
	PST_PRINT_DROP,
	PST_PRINT_PAUSELIFT,
	
	PST_PRINT_LASTLAYERLIFT,
	
} PRINTER_STATE;



#pragma pack(1)
typedef struct
{
	uint16_t		cfg_crc;
	uint16_t		cfg_version;

	uint8_t			invert_dir;
	int8_t			home_dir;
	float			home_pos;
	float			min_pos;
	float			max_pos;
	uint8_t			min_endstop_inverting;
	uint8_t			max_endstop_inverting;

	float			steps_per_mm;
	float			max_feedrate;
	float			max_acceleration;
	float			min_feedrate;
	float			min_travel_feedrate;
	float			max_jerk;

	float			acceleration;
	float			feedrate;
	float			travel_acceleration;
	float			travel_feedrate;
	float			homing_feedrate_fast;
	float			homing_feedrate_slow;
	
	float			current_vref;
	float			current_hold_vref;
	uint32_t		hold_time;		// time after the last movement until the motor current decreases by half, in msec
	uint32_t		off_time;

} MOTOR_CONFIG;

typedef struct
{
	uint16_t		cfg_crc;
	uint16_t		cfg_version;

	uint8_t			language;
	float			zero_pos;
	float			pause_lift;
	float			end_lift;
	uint8_t			buzzer;
	uint16_t		buzzer_touch;
	uint16_t		buzzer_msg;
	uint32_t		screensaver_time;
	uint8_t			screensaver_type;
	uint8_t			display_rotate;
	uint8_t			use_ext_clock;

} GLOBAL_CONFIG;

typedef struct
{
	uint16_t		cfg_crc;
	uint32_t		cfg_version;

	uint32_t		led_time;
	uint32_t		disp_time;
	uint32_t		fan_time;
	uint32_t		total_print_time;
	uint32_t		total_on_time;

} WORK_TIME;

#pragma pack()




typedef struct
{
	PRINTER_STATE	printer_state;
	int32_t			current_position_steps;
	float			target_position;
	uint8_t			position_known;
	uint8_t			zmotor_enabled;
	uint32_t		timerWorkTimeSecs;
	uint8_t			print_is_homing;
	uint8_t			print_is_printing;
	uint8_t			print_is_paused;
	uint8_t			print_is_canceled;
	uint32_t		print_time_begin;
	uint32_t		print_pause_time;
	uint32_t		print_current_layer;
	uint32_t		print_current_sublayer;
	float			print_current_height;
	float			print_light_time_total;
	float			print_lift_at_end;
	uint8_t			print_use_aa_grade;
	uint8_t			print_use_ind_params;
	uint32_t		print_timer_secs;
} SYSTEM_INFO;



typedef enum
{
	PARAMVAL_NONE = 0,
	PARAMVAL_NUMERIC,
	PARAMVAL_STRING
} VALUE_TYPE;

typedef struct
{
	float			float_val;
	int32_t			int_val;
	uint32_t		uint_val;
	char			*char_val;
	VALUE_TYPE		type;
} PARAM_VALUE;


typedef enum
{
	CFGR_ERROR = 0,
	CFGR_GENERAL,
	CFGR_ZMOTOR,
} CFGREAD_STATE;



extern SYSTEM_INFO			systemInfo;
extern MOTOR_CONFIG			cfgzMotor;
extern GLOBAL_CONFIG		cfgConfig;
extern WORK_TIME			cfgTimers;
extern char					cfgCFileName[512];
extern TCHAR				cfgTFileName[512];



void			CFG_Init();

uint16_t		CFG_MotorCalculateCRC();
uint16_t		CFG_ConfigCalculateCRC();
uint16_t		CFG_TimersCalculateCRC();

void			CFG_SetMotorDefault();
void			CFG_RefreshMotor();
void			CFG_SaveMotor();

void			CFG_SetConfigDefault();
void			CFG_RefreshConfig();
void			CFG_SaveConfig();

void			CFG_SetTimersDefault();
void			CFG_RefreshTimers();
void			CFG_SaveTimers();

void			_cfg_GetParamName(char *src, char *dest, uint16_t maxlen);
void			_cfg_GetParamValue(char *src, PARAM_VALUE *val);
void			CFG_LoadFromFile(void *par1, void *par2);

	

#endif /*__config_H */

