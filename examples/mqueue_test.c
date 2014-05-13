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
#include  <conio.h>

#include  <windows.h>
#include  <mmsystem.h>

#include  <raw_api.h>
#include  <mm/raw_page.h>
#include  <mm/raw_malloc.h>

#pragma comment( lib, "Winmm.lib" )



RAW_U16 raw_os_start();



#define  TEST_TASK_STK_SIZE 10240

PORT_STACK test_queue_stack1_mqueue[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack2_mqueue[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack3_mqueue[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack4_mqueue[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack5_mqueue[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 		test_queue_obj_mqueue[10];



volatile unsigned long  t22_mqueue;
volatile unsigned long  t33_mqueue;
volatile unsigned long  t44_mqueue;
volatile unsigned long  t55_mqueue;
extern void  vc_port_printf(char*   f,   ...);
void start_vc_timer(int tick_ms);


RAW_MQUEUE q1_mqueue;


extern RAW_U32 test_page_mem[1024*1024];


RAW_VOID *test_mqueue_msg[1];


void test_queue1_mqueue(void * pParam) 
{	
	
	int c = 2;
	int d = 1000;
	
	raw_page_init(test_page_mem, test_page_mem + 1024*1024);

	raw_malloc_init();

	raw_mq_init(&q1_mqueue, "mqueue", raw_malloc, raw_free, (RAW_MQUEUE_MSG **)&test_mqueue_msg, 100);

	
	while(1) {
		
		c++;
		d++;
		
		raw_mq_send(&q1_mqueue, &c, 4, 0, WAKE_ONE_MQUEUE, RAW_WAIT_FOREVER);

		vc_port_printf("1111*\n");
		
		raw_mq_send(&q1_mqueue, &d, 4, 0, WAKE_ONE_MQUEUE, RAW_WAIT_FOREVER);
		vc_port_printf("2222\n");
		raw_sleep(1000);
		
		
	}

	
}




void test_queue2_mqueue(void * pParam) 
{	
	
	int *a;
	int *b;
	RAW_U32 msg_length;
	RAW_U32 pri;
	
	while(1) {
			
		vc_port_printf("333333333\n");
		
		raw_mq_receive (&q1_mqueue,  &a, &msg_length, &pri,  RAW_WAIT_FOREVER);
		vc_port_printf("*a is %x\n", *a);
		
		raw_mq_receive (&q1_mqueue,  &b, &msg_length, &pri,  RAW_WAIT_FOREVER);

		
		vc_port_printf("*b is %x\n", *b);
		
		
	}
}



void test_queue3_mqueue(void * pParam) 
{	
	
	while(1)
	{		
		raw_sleep(1000);
	
	}
}


void test_queue4_mqueue(void * pParam) 
{	
	
	while(1)
	{		
		
    	raw_sleep(1000); 	
		
	}
}



void test_queue5_mqueue(void * pParam) 
{	
	
	while(1)
	{		
		raw_sleep(1000);
        	
	}
}

extern RAW_U8 test_started_raw;

void queue_test_mqueue()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	raw_task_create(&test_queue_obj_mqueue[1], (RAW_U8  *)"task1", 0,
	                         3, 0,  test_queue_stack1_mqueue, 
	                         TEST_TASK_STK_SIZE ,  test_queue1_mqueue, 1); 


	raw_task_create(&test_queue_obj_mqueue[2], (RAW_U8  *)"task2", 0,
	                         40, 10,   test_queue_stack2_mqueue, 
	                         TEST_TASK_STK_SIZE ,  test_queue2_mqueue, 1); 



  
	raw_task_create(&test_queue_obj_mqueue[3], (RAW_U8  *)"task3", 0,
	                         40, 10,   test_queue_stack3_mqueue, 
	                         TEST_TASK_STK_SIZE ,  test_queue3_mqueue, 1); 

	raw_task_create(&test_queue_obj_mqueue[4], (RAW_U8  *)"task4", 0,
	                         40, 10,   test_queue_stack4_mqueue, 
	                         TEST_TASK_STK_SIZE ,  test_queue4_mqueue, 1); 

	raw_task_create(&test_queue_obj_mqueue[5], (RAW_U8  *)"task5", 0,
	                         40, 10,   test_queue_stack5_mqueue, 
	                         TEST_TASK_STK_SIZE ,  test_queue5_mqueue, 1); 
 
	
	

}

