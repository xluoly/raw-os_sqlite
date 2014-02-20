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

PORT_STACK test_queue_size_stack1[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_size_stack2[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_size_stack3[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_size_stack4[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_size_stack5[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ test_queue_size_obj[10];



volatile unsigned long  t22_size;
volatile unsigned long  t33_size;
volatile unsigned long  t44_size;
volatile unsigned long  t55_size;

extern void  vc_port_printf(char*   f,   ...);
void start_vc_timer(int tick_ms);


RAW_QUEUE_SIZE q1_size;
RAW_MSG_SIZE  test_queue_size[1];

void test_queue_size1(void * pParam) 
{	
	
	int c = 2;
	int d = 1000;

	raw_queue_size_create(&q1_size, "queue1", test_queue_size, 1);
	
	while(1) {
		
		c++;
		d++;
		raw_queue_size_end_post(&q1_size, &c, 4, RAW_WAIT_FOREVER);

		vc_port_printf("1111*\n");
		raw_queue_size_end_post(&q1_size, &d, 8, RAW_WAIT_FOREVER);
		vc_port_printf("2222\n");
		raw_sleep(1000);
		
		
	}

	
}




void test_queue_size2(void * pParam) 
{	
	
	int *a;
	int *b;

	RAW_U32 a_size;
	RAW_U32 b_size;
	
	while(1) {
		
		a_size = b_size = 0;
		
		vc_port_printf("333333333\n");
		raw_queue_size_receive (&q1_size, RAW_WAIT_FOREVER, &a, &a_size);
		vc_port_printf("*a is %x\n", *a);
		raw_queue_size_receive (&q1_size, RAW_WAIT_FOREVER, &b, &b_size);

		vc_port_printf("*b is %x\n", *b);
		vc_port_printf("a size is %d b size is %d\n", a_size, b_size);
		
		
	}
}



void test_queue_size3(void * pParam) 
{	
	
	while(1)
	{		
		raw_sleep(1000);
	
	}
}


void test_queue_size4(void * pParam) 
{	
	
	while(1)
	{		
		
        raw_sleep(1000);	
		
	}
}



void test_queue_size5(void * pParam) 
{	
	
	while(1)
	{		
		raw_sleep(1000);
        	
	}
}

extern RAW_U8 test_started_raw;

void queue_size_full_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	raw_task_create(&test_queue_size_obj[1], (RAW_U8  *)"task1", 0,
	                         3, 0,  test_queue_size_stack1, 
	                         TEST_TASK_STK_SIZE ,  test_queue_size1, 1); 


	raw_task_create(&test_queue_size_obj[2], (RAW_U8  *)"task2", 0,
	                         40, 10,   test_queue_size_stack2, 
	                         TEST_TASK_STK_SIZE ,  test_queue_size2, 1); 



  
	raw_task_create(&test_queue_size_obj[3], (RAW_U8  *)"task3", 0,
	                         40, 10,   test_queue_size_stack3, 
	                         TEST_TASK_STK_SIZE ,  test_queue_size3, 1); 

	raw_task_create(&test_queue_size_obj[4], (RAW_U8  *)"task4", 0,
	                         40, 10,   test_queue_size_stack4, 
	                         TEST_TASK_STK_SIZE ,  test_queue_size4, 1); 

	raw_task_create(&test_queue_size_obj[5], (RAW_U8  *)"task5", 0,
	                         40, 10,   test_queue_size_stack5, 
	                         TEST_TASK_STK_SIZE ,  test_queue_size5, 1); 
 
	
}

