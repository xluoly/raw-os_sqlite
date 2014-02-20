/*
     raw os - Copyright (C)  Lingjun Chen(jorya_txj).

    This file is part of raw os.

    raw os is free software; you can redistribute it it under the terms of the 
    GNU General Public License as published by the Free Software Foundation; 
    either version 3 of the License, or  (at your option) any later version.

    raw os is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
    without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. if not, write email to jorya.txj@gmail.com
                                      ---

    A special exception to the LGPL can be applied should you wish to distribute
    a combined work that includes raw os, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/


/* 	2013-6  Created by jorya_txj
  *	xxxxxx   please added here
  */


#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include	<conio.h>

#include	<windows.h>
#include	<mmsystem.h>
#include <TCHAR.h>
#include <strsafe.h>

#include <raw_api.h>
#include <rtc_config.h>
#include <rtc.h>

#pragma comment( lib, "Winmm.lib" )


#define  TEST_TASK_STK_SIZE 2048

static RAW_TASK_OBJ  rtc_task_obj2;
static PORT_STACK test_task_rtc[TEST_TASK_STK_SIZE];

CLK_DATE_TIME date_time;
CLK_DATE_TIME date_time2;

static void task_rtc(void * pParam)
{
	RAW_U8 valid;

	CLK_TZ_SEC tz_sec;
	CLK_TS_SEC ts_sec;
	CPU_CHAR str[128];
	CLK_TS_SEC ts_unix_sec;
	
	tz_sec = 0;
	
	valid = Clk_DateTimeMake(&date_time, 2013, 8, 11, 01, 11, 11, tz_sec);
	if (valid != DEF_OK) {
		vc_port_printf("Clock make date/time failed\n\r");
	
	}


	valid = Clk_SetDateTime(&date_time);
	
	if (valid != DEF_OK) {
		vc_port_printf("Clock set date/time failed\n\r");
	
	}


	Clk_DateTimeToTS(&ts_sec, &date_time);
	
	if (valid == DEF_OK) {
		vc_port_printf("Clock timestamp = %u\n\r", ts_sec);
	} else {
		vc_port_printf("Clock date/time to timestamp failed\n\r");
	
	}


	tz_sec = (-5 * 60 * 60);
	valid = Clk_SetTZ(tz_sec);
	
	if (valid != DEF_OK) {
		
		vc_port_printf("Clock set timezone unix failed\n\r");
		
		return;
		
	}

	ts_sec = Clk_GetTS(); 
	
	valid = Clk_TS_ToDateTime(ts_sec, 0, &date_time); 
	if (valid != DEF_OK) {
		vc_port_printf("Clock convert timestamp to date/time failed\n\r");
	
	}
	valid = Clk_GetTS_Unix(&ts_unix_sec); 
	
	if (valid != DEF_OK) {
		vc_port_printf("Clock get timestamp unix failed\n\r");
	
	}
	
	valid = Clk_TS_UnixToDateTime(ts_unix_sec, tz_sec, &date_time); 
	if (valid != DEF_OK) {
		vc_port_printf("Clock timestamp unix to date/time failed\n\r");
	
	}
	
	valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS, str, 128);
	if (valid == DEF_OK) {
		vc_port_printf("Current Date/time :%s", str);
	} else {
		vc_port_printf("Clock date/time to string failed\n\r");

	}
	ts_unix_sec = 126316799uL;
	
	valid = Clk_TS_UnixToDateTime(ts_unix_sec, tz_sec, &date_time);
	if (valid != DEF_OK) {
		vc_port_printf("Clock set date/time failed\n\r");
		
	}


	valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_DAY_MONTH_DD_HH_MM_SS_YYYY, str, 128);
	if (valid == DEF_OK) {
		vc_port_printf("Unix timestamp = %s", str);
	} else {
		vc_port_printf("Clock date/time to string failed\n\r");
	
	}
	
	while (1) {

		Clk_GetDateTime(&date_time);
		raw_sleep(RAW_TICKS_PER_SECOND);
		vc_port_printf("seconds is %d\n", date_time.Sec);

		valid = Clk_DateTimeToStr(&date_time, CLK_STR_FMT_YYYY_MM_DD_HH_MM_SS,str, 128);
		
		if (valid == DEF_OK) {
			vc_port_printf("Current Date/time :%s", str);
		} else {
			vc_port_printf("Clock date/time to string failed\n\r");
		
		}
	}

}


extern RAW_U8 test_started_raw;

void rtc_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	Clk_Init();

	raw_task_create(&rtc_task_obj2, (RAW_U8  *)"task_rtc2", 0,
	                         3, 0,  test_task_rtc, 
	                         TEST_TASK_STK_SIZE ,  task_rtc, 1); 

	

}


