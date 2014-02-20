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

PORT_STACK test_timer_stack1[TEST_TASK_STK_SIZE];
PORT_STACK test_timer_stack2[TEST_TASK_STK_SIZE];
PORT_STACK test_timer_stack3[TEST_TASK_STK_SIZE];
PORT_STACK test_timer_stack4[TEST_TASK_STK_SIZE];
PORT_STACK test_timer_stack5[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 		test_timer_obj[10];


extern RAW_U32 raw_tick_count;

volatile unsigned long  t22;
volatile unsigned long  t33;
volatile unsigned long  t44;
volatile unsigned long  t55;

extern void  vc_port_printf(char*   f,   ...);
void start_vc_timer(int tick_ms);

RAW_TIMER timer_test1;
RAW_TIMER timer_test2;

RAW_U32 test_timer_count;
RAW_U32 test_timer_count2;


void timer_test_fun(void *para)
{

	vc_port_printf("raw_timer_count 1111* is %d\n", raw_timer_count);
	
}


void timer_test_fun2(void *para)
{


	vc_port_printf("raw_timer_count 2222* is %d\n", raw_timer_count);
	
}


void test_timer1(void * pParam) 
{	
	
	vc_port_printf("start timer test\n");
	raw_timer_create(&timer_test1, (RAW_U8  *)"timer1", timer_test_fun, 0, 20, 20, 0);
	raw_timer_create(&timer_test2, (RAW_U8  *)"timer2", timer_test_fun2, 0, 20, 20, 0);
	raw_timer_activate(&timer_test1, 0);
	raw_timer_activate(&timer_test2, 0);

	
	while(1)
	{		
		
		raw_sleep(5 * RAW_TICKS_PER_SECOND);	
		raw_timer_deactivate(&timer_test1);
		raw_sleep(5 * RAW_TICKS_PER_SECOND);
		raw_timer_activate(&timer_test1, 0);

		
	}
}




void test_timer2(void * pParam) 
{	
	
	while(1)
	{		
	
	}
}



void test_timer3(void * pParam) 
{	
	
	while(1)
	{		
		
	
	}
}


void test_timer4(void * pParam) 
{	
	
	while(1)
	{		
		
        	
		
	}
}



void test_timer5(void * pParam) 
{	
	
	while(1)
	{		
		
        	
	}
}

extern RAW_U8 test_started_raw;

void timer_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;


	raw_task_create(&test_timer_obj[1], (RAW_U8  *)"task1", 0,
	                         10, 0,  test_timer_stack1, 
	                         TEST_TASK_STK_SIZE ,  test_timer1, 1); 


	raw_task_create(&test_timer_obj[2], (RAW_U8  *)"task2", 0,
	                         40, 10,   test_timer_stack2, 
	                         TEST_TASK_STK_SIZE ,  test_timer2, 1); 


	#if 0 

	raw_task_create(&test_task_obj[3], (RAW_U8  *)"task3", 0,
	                         40, 10,   test_task_stack3, 
	                         TEST_TASK_STK_SIZE ,  test_task3); 

	raw_task_create(&test_task_obj[4], (RAW_U8  *)"task4", 0,
	                         40, 10,   test_task_stack4, 
	                         TEST_TASK_STK_SIZE ,  test_task4); 

	raw_task_create(&test_task_obj[5], (RAW_U8  *)"task5", 0,
	                         40, 10,   test_task_stack5, 
	                         TEST_TASK_STK_SIZE ,  test_task5); 
  #endif

	

}

