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


/* 	2012-8  Created by jorya_txj
  *	xxxxxx   please added here
  */


#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include	<conio.h>

#include	<windows.h>
#include	<mmsystem.h>

#include <raw_api.h>

#pragma comment( lib, "Winmm.lib" )



RAW_U16 raw_os_start();



#define  TEST_TASK_STK_SIZE 10240

PORT_STACK test_mutex_stack1[TEST_TASK_STK_SIZE];
PORT_STACK test_mutex_stack2[TEST_TASK_STK_SIZE];
PORT_STACK test_mutex_stack3[TEST_TASK_STK_SIZE];
PORT_STACK test_mutex_stack4[TEST_TASK_STK_SIZE];
PORT_STACK test_mutex_stack5[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 		test_mutex_obj[10];




extern void  vc_port_printf(char*   f,   ...);
void start_vc_timer(int tick_ms);


RAW_MUTEX mutext1;


void test_mutex1(void * pParam) 
{	
	
	raw_mutex_create(&mutext1, (RAW_U8 *)"mutex1", RAW_MUTEX_INHERIT_POLICY, 0);
	//raw_mutex_create(&mutext1, (RAW_U8 *)"mutex1", RAW_MUTEX_CEILING_POLICY, 9);
	//raw_mutex_create(&mutext1, (RAW_U8 *)"mutex1", RAW_MUTEX_NONE_POLICY, 9);
	
	while(1)
	{		
		
		raw_sleep(RAW_TICKS_PER_SECOND);

		vc_port_printf("..............................\n");
		
		raw_mutex_get(&mutext1, 2);
		vc_port_printf("test_mutex1\n");
		
		raw_mutex_put(&mutext1);
		
	}
}




void test_mutex2(void * pParam) 
{	
	
	while(1) {

		raw_sleep(4 * RAW_TICKS_PER_SECOND);
		vc_port_printf("******************11\n");
	
	
	}
	
}



void test_mutex3(void * pParam) 
{	
	
	while(1) {
		
		raw_mutex_get(&mutext1, RAW_WAIT_FOREVER);
		raw_mutex_get(&mutext1, RAW_WAIT_FOREVER);

		vc_port_printf("test_mutex3\n");

		raw_sleep(RAW_TICKS_PER_SECOND*2);
		vc_port_printf("test_mutex333333\n");
		raw_mutex_put(&mutext1);
		raw_mutex_put(&mutext1);
		
	}
}


extern RAW_U8 test_started_raw;

void mutex_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	raw_task_create(&test_mutex_obj[1], (RAW_U8  *)"mutex_task1", 0,
	                         10, 0,  test_mutex_stack1, 
	                         TEST_TASK_STK_SIZE ,  test_mutex1, 1); 


	raw_task_create(&test_mutex_obj[2], (RAW_U8  *)"mutex_task2", 0,
	                         30, 10,   test_mutex_stack2, 
	                         TEST_TASK_STK_SIZE ,  test_mutex2, 1); 

	raw_task_create(&test_mutex_obj[3], (RAW_U8  *)"mutex_task3", 0,
	                         40, 10,   test_mutex_stack3, 
	                         TEST_TASK_STK_SIZE ,  test_mutex3, 1); 


}
