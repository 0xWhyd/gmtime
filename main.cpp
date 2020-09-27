/*----------------------------------------------------------------------------------------------------*-

Автор:
	Fro он же Fro1sha
	
Описание:
	GMT время в SA:MP
 
Права: 
    Copyright © 2009-2011 TBG
	


                            ______              __________             
                            ___  /______  __    ___  ____/____________ 
                            __  __ \_  / / /    __  /_   __  ___/  __ \
                            _  /_/ /  /_/ /     _  __/   _  /   / /_/ /
                            /_.___/_\__, /      /_/      /_/    \____/ 
                                   /____/                              
                                                                              
																				
           _______________       _________                                              
           ___  __/__  __ )      __  ____/_____ _______ ________________ ___________  __
           __  /  __  __  |_______  / __ _  __ `/_  __ `__ \  _ \_  ___/ __  ___/  / / /
           _  /   _  /_/ /_/_____/ /_/ / / /_/ /_  / / / / /  __/(__  )___  /   / /_/ / 
           /_/    /_____/        \____/  \__,_/ /_/ /_/ /_/\___//____/_(_)_/    \__,_/  
                                                                                        

	http://tb-games.ru/
	
-*----------------------------------------------------------------------------------------------------*/

// -------------------------------------------------------------------------------------*-

#define PLUGIN_VERSION							"0.2.3"

// -------------------------------------------------------------------------------------*-


#ifdef WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

// -------------------------------------------------------------------------------------*-

typedef void (* logprintf_t)(char *, ...);

// -------------------------------------------------------------------------------------*-

#define CHECK_PARAMS(m, n) \
	if (params[0] != (m * 4)) \
	{ \
		logprintf("Erorr native function %s: Expecting %d parameter(s), but found %d", n, m, params[0] / 4); \
		return 0; \
	}

// -------------------------------------------------------------------------------------*-

#define IsLeapYear(year) \
    ((!((year) & 0x03)) && (((year) % 100) || !((year) % 400)))

// -------------------------------------------------------------------------------------*-

logprintf_t
	logprintf;

extern void *
	pAMXFunctions;

// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT unsigned int PLUGIN_CALL
	Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT bool PLUGIN_CALL
	Load(void ** ppData)
{	
/*
#ifdef WIN32
	_putenv("TZ=PST+00:00");
	_tzset();
#else
	putenv("TZ=PST+00:00");
	tzset();
#endif
*/
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	logprintf("\n");
	logprintf("______________________________________\n");
	logprintf(" GMT Time Plugin v"PLUGIN_VERSION" loaded");
	logprintf("______________________________________\n");
	logprintf(" By: Fro (c) Copyright <TBG> 2009-2011");
	logprintf("______________________________________\n");
	return true;
}

// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT void PLUGIN_CALL
	Unload()
{
	logprintf("\n");
	logprintf("______________________________________\n");
	logprintf(" GMT Time Plugin v"PLUGIN_VERSION" unloaded");
	logprintf("______________________________________\n");
}

// -------------------------------------------------------------------------------------*-

static cell AMX_NATIVE_CALL
	n_gm_SetTimeZone(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "gm_SetTimeZone");

	char
		tzone[ 12 ]
	;

	sprintf(tzone, "TZ=PST%02d:%02d", -(int)params[1], (int)params[2]);
#ifdef WIN32
	_putenv(tzone);
	_tzset();
#else
	putenv(tzone);
	tzset();
#endif
	return 1;
}

// -------------------------------------------------------------------------------------*-

static cell AMX_NATIVE_CALL
	n_gm_gettime(AMX * amx, cell * params)
{
	CHECK_PARAMS(3, "gm_gettime");

	cell
		* hour,
		* minute,
		* second
	;

	amx_GetAddr(amx, params[1], &hour);
	amx_GetAddr(amx, params[2], &minute);
	amx_GetAddr(amx, params[3], &second);

	time_t
		rawtime
	;
	tm
		* timeinfo
	;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	* hour = timeinfo->tm_hour;
	* minute = timeinfo->tm_min;
	* second = timeinfo->tm_sec;

	return (cell)rawtime;
}

// -------------------------------------------------------------------------------------*-

static cell AMX_NATIVE_CALL
	n_gm_getdate(AMX * amx, cell * params)
{
	CHECK_PARAMS(4, "gm_getdate");

	cell
		* year,
		* month,
		* day,
		* wday
	;

	amx_GetAddr(amx, params[1], &year);
	amx_GetAddr(amx, params[2], &month);
	amx_GetAddr(amx, params[3], &day);
	amx_GetAddr(amx, params[4], &wday);

	time_t
		rawtime
	;
	tm
		* timeinfo
	;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	* year = timeinfo->tm_year + 1900;
	* month = timeinfo->tm_mon + 1;
	* day = timeinfo->tm_mday;
	* wday = timeinfo->tm_wday;
	
	return (cell)timeinfo->tm_yday+1;
}

// -------------------------------------------------------------------------------------*-

static cell AMX_NATIVE_CALL
	n_gm_strftime( AMX * amx, cell * params )
{
	CHECK_PARAMS( 3, "gm_strftime" );

	char
		* string,
		* buffer = (char *)malloc( params[ 3 ] )
	;

	time_t rawtime;
	tm * timeinfo;

	amx_StrParam( amx, params[ 1 ], string );

	if ( string && buffer )
	{
		time( &rawtime );
		timeinfo = localtime( &rawtime );

		strftime( buffer, params[ 3 ], string, timeinfo );

		cell * buf;

		amx_GetAddr( amx, params[ 2 ], &buf );
		amx_SetString( buf, buffer, 0, 0, params[ 3 ] );
		free( buffer );
		return 1;
	}
	return 0;
}

// -------------------------------------------------------------------------------------*-

static cell AMX_NATIVE_CALL
	n_gm_timestamp(AMX * amx, cell * params)
{
	return (cell)time(NULL);
}

// -------------------------------------------------------------------------------------*-

static cell AMX_NATIVE_CALL
	n_gm_mktime(AMX * amx, cell * params)
{
	CHECK_PARAMS(6, "gm_mktime");

	struct tm 
		timeinfo
	;

	timeinfo.tm_hour = (int)params[1];
	timeinfo.tm_min = (int)params[2];
	timeinfo.tm_sec = (int)params[3];
	timeinfo.tm_year = (int)params[4] - 1900;
	timeinfo.tm_mon = (int)params[5] - 1;
	timeinfo.tm_mday = (int)params[6];

    timeinfo.tm_isdst = -1;
	timeinfo.tm_wday  = -1;
    timeinfo.tm_yday  = -1;

	return (cell)mktime(&timeinfo);
}

// -------------------------------------------------------------------------------------*-

static cell AMX_NATIVE_CALL
	n_gm_timestamp_to_date(AMX * amx, cell * params)
{
	CHECK_PARAMS(7, "gm_timestamp_to_date");

	cell
		* hour,
		* minute,
		* second,
		* year,
		* month,
		* day
	;
	time_t
		rawtime = (int)params[1]
	;
	tm
		* timeinfo
	;

	timeinfo = localtime(&rawtime);

	amx_GetAddr(amx, params[2], &hour);
	amx_GetAddr(amx, params[3], &minute);
	amx_GetAddr(amx, params[4], &second);
	amx_GetAddr(amx, params[5], &year);
	amx_GetAddr(amx, params[6], &month);
	amx_GetAddr(amx, params[7], &day);

	* hour = timeinfo->tm_hour;
	* minute = timeinfo->tm_min;
	* second = timeinfo->tm_sec;

	* year = timeinfo->tm_year + 1900;
	* month = timeinfo->tm_mon + 1;
	* day = timeinfo->tm_mday;
	return 1;
}

// -------------------------------------------------------------------------------------*-

static cell AMX_NATIVE_CALL
	n_gm_GetMonthDay(AMX * amx, cell * params)
{
	CHECK_PARAMS(2, "gm_GetMonthDay");

	switch ( (int)params[1] )
	{
		case 1: return 31;
		case 2: return ( IsLeapYear((int)params[2]) ? 29 : 28 );
		case 3: return 31;
		case 4: return 30;
		case 5: return 31;
		case 6: return 30;
		case 7: return 31;
		case 8: return 31;
		case 9: return 30;
		case 10: return 31;
		case 11: return 30;
		case 12: return 31;
	}

	return 0;
}

// -------------------------------------------------------------------------------------*-

static cell AMX_NATIVE_CALL
	n_gm_ConvertSeconds(AMX * amx, cell * params)
{
	CHECK_PARAMS(7, "gm_ConvertSeconds");

	cell
		* hour,
		* minute,
		* second,
		* year,
		* month,
		* day
	;
	int
		time = (int)params[1]
	;

	amx_GetAddr(amx, params[2], &hour);
	amx_GetAddr(amx, params[3], &minute);
	amx_GetAddr(amx, params[4], &second);
	amx_GetAddr(amx, params[5], &year);
	amx_GetAddr(amx, params[6], &month);
	amx_GetAddr(amx, params[7], &day);

	* hour = (time / 3600) % 24;
	* minute = (time / 60) % 60;
	* second = time % 60;

	* year = time / 31104000;
	* month = (time / 2592000) % 12;
	* day = (time / 86400) % 30;
	return 1;
}

// -------------------------------------------------------------------------------------*-

AMX_NATIVE_INFO
	gmtime_natives[] =
{
	{ "gm_SetTimeZone", n_gm_SetTimeZone },
	{ "gm_gettime", n_gm_gettime },
	{ "gm_getdate", n_gm_getdate },
	{ "gm_strftime", n_gm_strftime },
	{ "gm_timestamp", n_gm_timestamp },
	{ "gm_mktime", n_gm_mktime },
	{ "gm_timestamp_to_date", n_gm_timestamp_to_date },
	{ "gm_GetMonthDay", n_gm_GetMonthDay },
	{ "gm_ConvertSeconds", n_gm_ConvertSeconds },
	{ 0, 0 }
};

// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT int PLUGIN_CALL
	AmxLoad(AMX * amx)
{
	return amx_Register(amx, gmtime_natives, -1);
}

// -------------------------------------------------------------------------------------*-

PLUGIN_EXPORT int PLUGIN_CALL
	AmxUnload(AMX * amx)
{
	return AMX_ERR_NONE;
}

// -------------------------------------------------------------------------------------*-