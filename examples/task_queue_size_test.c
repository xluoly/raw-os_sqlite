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


/* 	2013-2  Created by jorya_txj
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

static PORT_STACK test_queue_stack11_size[TEST_TASK_STK_SIZE];
static PORT_STACK test_queue_stack22_size[TEST_TASK_STK_SIZE];
static PORT_STACK test_queue_stack33_size[TEST_TASK_STK_SIZE];
static PORT_STACK test_queue_stack44_size[TEST_TASK_STK_SIZE];
static PORT_STACK test_queue_stack55_size[TEST_TASK_STK_SIZE];

static RAW_TASK_OBJ  test_queue_obj1_size[10];

extern void  vc_port_printf(char*   f,   ...);

static RAW_QUEUE_SIZE app_queue2;
static RAW_QUEUE_SIZE app_queue3;
static RAW_QUEUE_SIZE app_queue4;

static RAW_MSG_SIZE queue1_storge_size[1000];
static RAW_MSG_SIZE queue2_storge_size[2000];
static RAW_MSG_SIZE queue3_storge_size[2000];




static RAW_S32	msg1_2;
static RAW_S32	msg2_2;
static RAW_S32	msg3_2;
static RAW_S32	msg4_2;
static RAW_S32	msg5_2;

static unsigned int watch_number_size;

void start_vc_timer(int tick_ms);

static void driver_send_task_size(void * pParam) 
{	
	msg1_2 = 0x188;
	msg2_2 = 0x188;
	msg3_2 = 0x188;
	msg4_2 = 0x188;
	msg5_2 = 0x188;
	
	vc_port_printf("complex task queue_size test..........\n");

	raw_task_qsize_create(&test_queue_obj1_size[2], &app_queue2, "driver_queue", queue1_storge_size, 1000);
	raw_task_qsize_create(&test_queue_obj1_size[3], &app_queue3, "driver_queue", queue2_storge_size, 2000);
	raw_task_qsize_create(&test_queue_obj1_size[4], &app_queue4, "driver_queue", queue3_storge_size, 2000);
	
	while(1) {
		
		raw_task_qsize_end_post(&test_queue_obj1_size[2], &msg1_2, 4);
		raw_task_qsize_end_post(&test_queue_obj1_size[2], &msg2_2, 4);
		raw_task_qsize_end_post(&test_queue_obj1_size[2], &msg3_2, 4);
		raw_task_qsize_end_post(&test_queue_obj1_size[2], &msg4_2, 4);
		raw_task_qsize_end_post(&test_queue_obj1_size[2], &msg5_2, 4);
		watch_number_size++;
		raw_sleep(10);
		
		
	}

	
}




static void  net_receive_task_size(void * pParam) 
{	

	void *msg_app1;
	RAW_U32 msg_app1_size;
	
	void *msg_app2;
	RAW_U32 msg_app2_size;
	void *msg_app3;
	RAW_U32 msg_app3_size;
	void *msg_app4;
	RAW_U32 msg_app4_size;
	void *msg_app5;
	RAW_U32 msg_app5_size;

	
	while(1) {
			
		raw_task_qsize_receive(RAW_WAIT_FOREVER, &msg_app1, &msg_app1_size);
		raw_task_qsize_end_post(&test_queue_obj1_size[3], msg_app1, 4);
		raw_task_qsize_end_post(&test_queue_obj1_size[4], msg_app1, 4);

		raw_task_qsize_receive(RAW_WAIT_FOREVER, &msg_app2, &msg_app2_size);
		raw_task_qsize_end_post(&test_queue_obj1_size[3], msg_app2, 4);
		raw_task_qsize_end_post(&test_queue_obj1_size[4], msg_app2, 4);
		
		raw_task_qsize_receive (RAW_WAIT_FOREVER, &msg_app3,&msg_app3_size);
		raw_task_qsize_end_post(&test_queue_obj1_size[3], msg_app3, 4);
		raw_task_qsize_end_post(&test_queue_obj1_size[4], msg_app3, 4);

		raw_task_qsize_receive (RAW_WAIT_FOREVER, &msg_app4, &msg_app4_size);
		raw_task_qsize_end_post(&test_queue_obj1_size[3], msg_app4, 4);
		raw_task_qsize_end_post(&test_queue_obj1_size[4], msg_app4, 4);

		raw_task_qsize_receive (RAW_WAIT_FOREVER, &msg_app5, &msg_app5_size);
		raw_task_qsize_end_post(&test_queue_obj1_size[3], msg_app5, 4);
		raw_task_qsize_end_post(&test_queue_obj1_size[4], msg_app5, 4);
		
		
		
	}
}



static void app_receive_task1_size(void * pParam) 
{	
	int *data1;
		int *data2;
			int *data3;
				int *data4;
					int *data5;
	RAW_U32 data1_size;
	RAW_U32 data2_size;
	RAW_U32 data3_size;
	RAW_U32 data4_size;
	RAW_U32 data5_size;
	
	while(1) {
			
		raw_task_qsize_receive(RAW_WAIT_FOREVER, &data1, &data1_size);
		
		if (*data1 != 0x188)
			RAW_ASSERT(0);
		
		raw_task_qsize_receive(RAW_WAIT_FOREVER, &data2, &data2_size);
		
		if (*data2 != 0x188)
			RAW_ASSERT(0);
		
		raw_task_qsize_receive(RAW_WAIT_FOREVER, &data3, &data3_size);

		if (*data3 != 0x188)
			RAW_ASSERT(0);
		
		raw_task_qsize_receive(RAW_WAIT_FOREVER, &data4, &data4_size);

		if (*data4 != 0x188)
			RAW_ASSERT(0);
		
		raw_task_qsize_receive(RAW_WAIT_FOREVER, &data5, &data5_size);

		if (*data5 != 0x188)
			RAW_ASSERT(0);
		
	
	}
}


static void app_receive_task2_size(void * pParam) 
{	
	int *data1;
		int *data2;
			int *data3;
				int *data4;
					int *data5;

	RAW_U32 data1_size;
	RAW_U32 data2_size;
	RAW_U32 data3_size;
	RAW_U32 data4_size;
	RAW_U32 data5_size;
	
	while(1)
	{		
		raw_task_qsize_receive(RAW_WAIT_FOREVER, &data1, &data1_size);
		
		if (*data1 != 0x188)
			RAW_ASSERT(0);
		
		raw_task_qsize_receive (RAW_WAIT_FOREVER, &data2, &data2_size);
		
		if (*data1 != 0x188)
			RAW_ASSERT(0);
		
		raw_task_qsize_receive (RAW_WAIT_FOREVER, &data3, &data3_size);

		if (*data3 != 0x188)
			RAW_ASSERT(0);
		
		raw_task_qsize_receive (RAW_WAIT_FOREVER, &data4, &data4_size);

		if (*data4 != 0x188)
			RAW_ASSERT(0);
		
		raw_task_qsize_receive (RAW_WAIT_FOREVER, &data5, &data5_size);

		if (*data5 != 0x188)
			RAW_ASSERT(0);
		
	
	}
}


static void watch_task_size(void * pParam) 
{
	while (1) {
		
		raw_sleep(RAW_TICKS_PER_SECOND / 2);
		vc_port_printf("watch11 number is %x\n", watch_number_size);
	}
	

}

extern RAW_U8 test_started_raw;

void task_queue_size_test()
{
	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;

	raw_task_create(&test_queue_obj1_size[1], (RAW_U8  *)"task1", 0,
	                         3, 0,  test_queue_stack11_size, 
	                         TEST_TASK_STK_SIZE ,  driver_send_task_size, 1); 


	raw_task_create(&test_queue_obj1_size[2], (RAW_U8  *)"task2", 0,
	                         11, 10,   test_queue_stack22_size, 
	                         TEST_TASK_STK_SIZE ,   net_receive_task_size, 1); 



  
	raw_task_create(&test_queue_obj1_size[3], (RAW_U8  *)"task3", 0,
	                         12, 10,   test_queue_stack33_size, 
	                         TEST_TASK_STK_SIZE ,  app_receive_task1_size, 1); 


	raw_task_create(&test_queue_obj1_size[4], (RAW_U8  *)"task4", 0,
	                         13, 10,   test_queue_stack44_size, 
	                         TEST_TASK_STK_SIZE ,  app_receive_task2_size, 1); 	


	raw_task_create(&test_queue_obj1_size[5], (RAW_U8  *)"task4", 0,
	                         14, 10,   test_queue_stack55_size, 
	                         TEST_TASK_STK_SIZE ,  watch_task_size, 1); 

	

}

