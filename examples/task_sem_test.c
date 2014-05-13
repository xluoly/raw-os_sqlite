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


/* 	2013-3  Created by jorya_txj
  *	xxxxxx   please added here
  */
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <conio.h>

#include  <windows.h>
#include  <mmsystem.h>
#include  <TCHAR.h>
#include  <strsafe.h>

#include  <raw_api.h>
#include  <simple_printf.h>

#pragma comment( lib, "Winmm.lib" )

extern RAW_U8 test_started_raw;


static RAW_TASK_OBJ  test_sem_obj[10];


#define  TEST_TASK_STK_SIZE (30 * 1024)

static PORT_STACK test_task_sem1[TEST_TASK_STK_SIZE];
static PORT_STACK test_task_sem2[TEST_TASK_STK_SIZE];

static RAW_SEMAPHORE s1;

static void task_sem1(void * pParam)
{
	
	raw_task_semaphore_create(&test_sem_obj[0], &s1, (RAW_U8 *)"sem", 0);
	
	while (1) {

		raw_task_semaphore_get(RAW_WAIT_FOREVER);
		vc_port_printf("raw_task_semaphore_get success\n");
		raw_printf("test_started_raw is %d\n", test_started_raw);

	}

}


static void task_sem2(void * pParam)
{
	while (1) {
		
		raw_task_semaphore_put(&test_sem_obj[0]);
		raw_sleep(100);
	}

}


void task_sem_test()
{
	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;

	
	raw_task_create(&test_sem_obj[0], (RAW_U8  *)"task1", 0,
	                         3, 0,  test_task_sem1, 
	                         TEST_TASK_STK_SIZE ,  task_sem1, 1); 


	raw_task_create(&test_sem_obj[1], (RAW_U8  *)"task2", 0,
	                         11, 10,   test_task_sem2, 
	                         TEST_TASK_STK_SIZE ,   task_sem2, 1); 


}

