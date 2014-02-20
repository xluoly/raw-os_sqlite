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
#include <TCHAR.h>
#include <strsafe.h>

#include <raw_api.h>

#pragma comment( lib, "Winmm.lib" )



RAW_U16 raw_os_start();



#define  TEST_TASK_STK_SIZE (30 * 1024)

PORT_STACK test_queue_stack11[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack22[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack33[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack44[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack55[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 		test_queue_obj1[10];



extern void  vc_port_printf(char*   f,   ...);
void start_vc_timer(int tick_ms);


RAW_QUEUE driver_queue;

RAW_QUEUE app_queue;
RAW_QUEUE app_queue2;

void *queue1_storge[1000];
void *queue2_storge[2000];
void *queue3_storge[2000];

int msg1;
int msg2;
int msg3;
int msg4;
int msg5;

unsigned int watch_number;

void driver_send_task(void * pParam) 
{	
	msg1 = 0x188;
	msg2 = 0x189;
	msg3 = 0x18a;
	msg4 = 0x18b;
	msg5 = 0x18c;

	vc_port_printf("complex queue test..........\n");
	
	raw_queue_create(&driver_queue, "driver_queue", (RAW_VOID **)queue1_storge, 1000);
	raw_queue_create(&app_queue, "driver_queue", (RAW_VOID **)queue2_storge, 2000);
	raw_queue_create(&app_queue2, "driver_queue", (RAW_VOID **)queue3_storge, 2000);
	
	while(1) {
		
		raw_queue_end_post(&driver_queue, &msg1);
		raw_queue_end_post(&driver_queue, &msg2);
		raw_queue_end_post(&driver_queue, &msg3);
		raw_queue_end_post(&driver_queue, &msg4);
		raw_queue_end_post(&driver_queue, &msg5);
		watch_number++;
		raw_sleep(10);
		
		
	}

	
}




void  net_receive_task(void * pParam) 
{	

	void *msg_app1;
	void *msg_app2;
	void *msg_app3;
	void *msg_app4;
	void *msg_app5;

	
	while(1)
	{		
		raw_queue_receive (&driver_queue, RAW_WAIT_FOREVER, &msg_app1);
		raw_queue_end_post(&app_queue, msg_app1);
		raw_queue_end_post(&app_queue2, msg_app1);

		raw_queue_receive (&driver_queue, RAW_WAIT_FOREVER, &msg_app2);
		raw_queue_end_post(&app_queue, msg_app2);
		raw_queue_end_post(&app_queue2, msg_app2);
		
		raw_queue_receive (&driver_queue, RAW_WAIT_FOREVER, &msg_app3);
		raw_queue_end_post(&app_queue, msg_app3);
		raw_queue_end_post(&app_queue2, msg_app3);

		raw_queue_receive (&driver_queue, RAW_WAIT_FOREVER, &msg_app4);
		raw_queue_end_post(&app_queue, msg_app4);
		raw_queue_end_post(&app_queue2, msg_app4);

		raw_queue_receive (&driver_queue, RAW_WAIT_FOREVER, &msg_app5);
		raw_queue_end_post(&app_queue, msg_app5);
		raw_queue_end_post(&app_queue2, msg_app5);
		
		
	}
}



void app_receive_task1(void * pParam) 
{	
	int *data1;
		int *data2;
			int *data3;
				int *data4;
					int *data5;
	
	while(1)
	{		
		raw_queue_receive (&app_queue, RAW_WAIT_FOREVER, &data1);
		
		if (*data1 != 0x188)
			RAW_ASSERT(0);
		
		raw_queue_receive (&app_queue, RAW_WAIT_FOREVER, &data2);
		
		if (*data2 != 0x189)
			RAW_ASSERT(0);
		
		raw_queue_receive (&app_queue, RAW_WAIT_FOREVER, &data3);

		if (*data3 != 0x18a)
			RAW_ASSERT(0);
		
		raw_queue_receive (&app_queue, RAW_WAIT_FOREVER, &data4);

		if (*data4 != 0x18b)
			RAW_ASSERT(0);
		
		raw_queue_receive (&app_queue, RAW_WAIT_FOREVER, &data5);

		if (*data5 != 0x18c)
			RAW_ASSERT(0);
		
	
	}
}


void app_receive_task2(void * pParam) 
{	
	int *data1;
		int *data2;
			int *data3;
				int *data4;
					int *data5;
	
	while(1)
	{		
		raw_queue_receive (&app_queue2, RAW_WAIT_FOREVER, &data1);
		
		if (*data1 != 0x188)
			RAW_ASSERT(0);
		
		raw_queue_receive (&app_queue2, RAW_WAIT_FOREVER, &data2);
		
		if (*data2 != 0x189)
			RAW_ASSERT(0);
		
		raw_queue_receive (&app_queue2, RAW_WAIT_FOREVER, &data3);

		if (*data3 != 0x18a)
			RAW_ASSERT(0);
		
		raw_queue_receive (&app_queue2, RAW_WAIT_FOREVER, &data4);

		if (*data4 != 0x18b)
			RAW_ASSERT(0);
		
		raw_queue_receive (&app_queue2, RAW_WAIT_FOREVER, &data5);

		if (*data5 != 0x18c)
			RAW_ASSERT(0);
		
	
	}
}


void watch_task(void * pParam) 
{
	while (1) {
		
		raw_sleep(RAW_TICKS_PER_SECOND / 10);
		vc_port_printf("watch number queue is %x\n", watch_number);
	}
	

}

extern RAW_U8 test_started_raw;

void queue_test2()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;

	
	
	raw_task_create(&test_queue_obj1[1], (RAW_U8  *)"task1", 0,
	                         2, 0,  test_queue_stack11, 
	                         TEST_TASK_STK_SIZE ,  driver_send_task, 1); 


	raw_task_create(&test_queue_obj1[2], (RAW_U8  *)"task2", 0,
	                         11, 10,   test_queue_stack22, 
	                         TEST_TASK_STK_SIZE ,   net_receive_task, 1); 



  
	raw_task_create(&test_queue_obj1[3], (RAW_U8  *)"task3", 0,
	                         12, 10,   test_queue_stack33, 
	                         TEST_TASK_STK_SIZE ,  app_receive_task1, 1); 


	raw_task_create(&test_queue_obj1[4], (RAW_U8  *)"task4", 0,
	                         13, 10,   test_queue_stack44, 
	                         TEST_TASK_STK_SIZE ,  app_receive_task2, 1); 	


	raw_task_create(&test_queue_obj1[5], (RAW_U8  *)"task4", 0,
	                         14, 10,   test_queue_stack55, 
	                         TEST_TASK_STK_SIZE ,  watch_task, 1); 

	

}

