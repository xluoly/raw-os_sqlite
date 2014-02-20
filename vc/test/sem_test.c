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

PORT_STACK test_sem_stack1[TEST_TASK_STK_SIZE];
PORT_STACK test_sem_stack2[TEST_TASK_STK_SIZE];
PORT_STACK test_sem_stack3[TEST_TASK_STK_SIZE];
PORT_STACK test_sem_stack4[TEST_TASK_STK_SIZE];
PORT_STACK test_sem_stack5[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 		test_sem_obj[10];



volatile unsigned long  t22;
volatile unsigned long  t33;
volatile unsigned long  t44;
volatile unsigned long  t55;
extern void  vc_port_printf(char*   f,   ...);
void start_vc_timer(int tick_ms);


RAW_SEMAPHORE s1;

void test_sem1(void * pParam) 
{	
	
	
	raw_semaphore_create(&s1, "sem1", 0);

	while(1)
	{	
		
		vc_port_printf("1111111\n");
		raw_semaphore_get(&s1,  RAW_WAIT_FOREVER);

		vc_port_printf("test1\n");
	
		
		
	}
}




void test_sem2(void * pParam) 
{	
	
	while(1) {
		vc_port_printf("222222222\n");
		raw_semaphore_get(&s1,  RAW_WAIT_FOREVER);
		vc_port_printf("test_sem2\n");
		//raw_sleep(100);
	}
}



void test_sem3(void * pParam) 
{	 
	while(1)
	{		
		raw_semaphore_put_all(&s1);
		raw_sleep(RAW_TICKS_PER_SECOND);
	
	}
}


void test_sem4(void * pParam) 
{	
	
	while(1)
	{		
		
        	
		
	}
}



void test_sem5(void * pParam) 
{	
	
	while(1)
	{		
		
        	
	}
}

extern RAW_U8 test_started_raw;

void sem_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	raw_task_create(&test_sem_obj[1], (RAW_U8  *)"task1", 0,
	                         10, 0,  test_sem_stack1, 
	                         TEST_TASK_STK_SIZE ,  test_sem1, 1); 


	raw_task_create(&test_sem_obj[2], (RAW_U8  *)"task2", 0,
	                         40, 10,   test_sem_stack2, 
	                         TEST_TASK_STK_SIZE ,  test_sem2, 1); 


	

	raw_task_create(&test_sem_obj[3], (RAW_U8  *)"task3", 0,
	                         41, 10,   test_sem_stack3, 
	                         TEST_TASK_STK_SIZE ,  test_sem3, 1); 

	#if 0 
	raw_task_create(&test_task_obj[4], (RAW_U8  *)"task4", 0,
	                         40, 10,   test_task_stack4, 
	                         TEST_TASK_STK_SIZE ,  test_task4); 

	raw_task_create(&test_task_obj[5], (RAW_U8  *)"task5", 0,
	                         40, 10,   test_task_stack5, 
	                         TEST_TASK_STK_SIZE ,  test_task5); 
  #endif

	

}

