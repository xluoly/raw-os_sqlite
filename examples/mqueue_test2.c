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


#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <conio.h>

#include  <windows.h>
#include  <mmsystem.h>
#include  <TCHAR.h>
#include  <strsafe.h>

#include  <raw_api.h>
#include  <mm/raw_page.h>
#include  <mm/raw_malloc.h>

#pragma comment( lib, "Winmm.lib" )



RAW_U16 raw_os_start();



#define  TEST_TASK_STK_SIZE (30 * 1024)

PORT_STACK test_queue_stack11_mqueue[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack22_mqueue[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack33_mqueue[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack44_mqueue[TEST_TASK_STK_SIZE];
PORT_STACK test_queue_stack55_mqueue[TEST_TASK_STK_SIZE];

RAW_TASK_OBJ 		test_queue_obj1_mqueue[10];



extern void  vc_port_printf(char*   f,   ...);
void start_vc_timer(int tick_ms);


RAW_MQUEUE driver_queue_mqueue;

RAW_MQUEUE app_queue_mqueue;
RAW_MQUEUE app_queue2_mqueue;


RAW_VOID *queue1_storge_mqueue[1000];
RAW_VOID *queue2_storge_mqueue[2000];
RAW_VOID *queue3_storge_mqueue[2000];



int msg1_mqueue;
int msg2_mqueue;
int msg3_mqueue;
int msg4_mqueue;
int msg5_mqueue;

unsigned int watch_number_mqueue;
extern RAW_U32 test_page_mem[1024*1024];

void driver_send_task_mqueue(void * pParam) 
{	
	msg1_mqueue = 0x188;
	msg2_mqueue = 0x188;
	msg3_mqueue = 0x188;
	msg4_mqueue = 0x188;
	msg5_mqueue = 0x188;

	vc_port_printf("complex mqueue test..........\n");
	
	raw_page_init(test_page_mem, test_page_mem + 1024*1024);

	raw_malloc_init();

	raw_mq_init(&driver_queue_mqueue, "mqueue", raw_malloc, raw_free, (RAW_VOID **)&queue1_storge_mqueue, 1000);
	raw_mq_init(&app_queue_mqueue, "mqueue", raw_malloc, raw_free, (RAW_VOID **)&queue2_storge_mqueue, 2000);
	raw_mq_init(&app_queue2_mqueue, "mqueue", raw_malloc, raw_free, (RAW_VOID **)&queue3_storge_mqueue, 2000);
	
	while(1) {

		raw_mq_send(&driver_queue_mqueue, &msg1_mqueue, 4, 0);
		raw_mq_send(&driver_queue_mqueue, &msg2_mqueue, 4, 0);
		raw_mq_send(&driver_queue_mqueue, &msg3_mqueue, 4, 0);
		raw_mq_send(&driver_queue_mqueue, &msg4_mqueue, 4, 0);
		raw_mq_send(&driver_queue_mqueue, &msg5_mqueue, 4, 0);
		
		watch_number_mqueue++;
		raw_sleep(10);
		
		
	}

	
}




void  net_receive_task_mqueue(void * pParam) 
{	

	void *msg_app1;
	void *msg_app2;
	void *msg_app3;
	void *msg_app4;
	void *msg_app5;


	RAW_U32 msg_length1;
	RAW_U32 msg_length2;
	RAW_U32 msg_length3;
	RAW_U32 msg_length4;
	RAW_U32 msg_length5;


	RAW_U32 pri1;
	RAW_U32 pri2;
	RAW_U32 pri3;
	RAW_U32 pri4;
	RAW_U32 pri5;
	
	while(1) {
		

		raw_mq_receive (&driver_queue_mqueue,  &msg_app1, &msg_length1, &pri1,  RAW_WAIT_FOREVER);
		raw_mq_send(&app_queue_mqueue, msg_app1, 4, 0);
		raw_mq_send(&app_queue2_mqueue, msg_app1, 4, 0);

		raw_mq_receive (&driver_queue_mqueue,  &msg_app2, &msg_length2, &pri2,  RAW_WAIT_FOREVER);
		raw_mq_send(&app_queue_mqueue, msg_app2, 4, 0);
		raw_mq_send(&app_queue2_mqueue, msg_app2, 4, 0);

		raw_mq_receive (&driver_queue_mqueue,  &msg_app3, &msg_length3, &pri3,  RAW_WAIT_FOREVER);
		raw_mq_send(&app_queue_mqueue, msg_app3, 4, 0);
		raw_mq_send(&app_queue2_mqueue, msg_app3, 4, 0);


		raw_mq_receive (&driver_queue_mqueue,  &msg_app4, &msg_length4, &pri4,  RAW_WAIT_FOREVER);
		raw_mq_send(&app_queue_mqueue, msg_app4, 4, 0);
		raw_mq_send(&app_queue2_mqueue, msg_app4, 4, 0);


		raw_mq_receive (&driver_queue_mqueue,  &msg_app5, &msg_length5, &pri5,  RAW_WAIT_FOREVER);
		raw_mq_send(&app_queue_mqueue, msg_app5, 4, 0);
		raw_mq_send(&app_queue2_mqueue, msg_app5, 4, 0);
		
		
	}
}



void app_receive_task1_mqueue(void * pParam) 
{	
	int *data1;
		int *data2;
			int *data3;
				int *data4;
					int *data5;

	RAW_U32 msg_length1;
	RAW_U32 msg_length2;
	RAW_U32 msg_length3;
	RAW_U32 msg_length4;
	RAW_U32 msg_length5;


	RAW_U32 pri1;
	RAW_U32 pri2;
	RAW_U32 pri3;
	RAW_U32 pri4;
	RAW_U32 pri5;
	
	while(1) {
		
		raw_mq_receive (&app_queue_mqueue,  &data1, &msg_length1, &pri1,  RAW_WAIT_FOREVER);
		
		if (*data1 != 0x188)
			RAW_ASSERT(0);
		
		raw_mq_receive (&app_queue_mqueue,  &data2, &msg_length2, &pri2,  RAW_WAIT_FOREVER);
		
		if (*data2 != 0x188)
			RAW_ASSERT(0);
		
		raw_mq_receive (&app_queue_mqueue,  &data3, &msg_length3, &pri3,  RAW_WAIT_FOREVER);

		if (*data3 != 0x188)
			RAW_ASSERT(0);
		
		raw_mq_receive (&app_queue_mqueue,  &data4, &msg_length4, &pri4,  RAW_WAIT_FOREVER);

		if (*data4 != 0x188)
			RAW_ASSERT(0);
		
		raw_mq_receive (&app_queue_mqueue,  &data5, &msg_length5, &pri5,  RAW_WAIT_FOREVER);

		if (*data5 != 0x188)
			RAW_ASSERT(0);
		
	
	}
}


void app_receive_task2_mqueue(void * pParam) 
{	
	int *data1;
		int *data2;
			int *data3;
				int *data4;
					int *data5;
					
	RAW_U32 msg_length1;
	RAW_U32 msg_length2;
	RAW_U32 msg_length3;
	RAW_U32 msg_length4;
	RAW_U32 msg_length5;


	RAW_U32 pri1;
	RAW_U32 pri2;
	RAW_U32 pri3;
	RAW_U32 pri4;
	RAW_U32 pri5;
	
	while(1) {
		
	
		raw_mq_receive (&app_queue2_mqueue,  &data1, &msg_length1, &pri1,  RAW_WAIT_FOREVER);
		
		if (*data1 != 0x188)
			RAW_ASSERT(0);
		
		raw_mq_receive (&app_queue2_mqueue,  &data2, &msg_length2, &pri2,  RAW_WAIT_FOREVER);
		
		if (*data2 != 0x188)
			RAW_ASSERT(0);
		
		raw_mq_receive (&app_queue2_mqueue,  &data3, &msg_length3, &pri3,  RAW_WAIT_FOREVER);

		if (*data3 != 0x188)
			RAW_ASSERT(0);
		
		raw_mq_receive (&app_queue2_mqueue,  &data4, &msg_length4, &pri4,  RAW_WAIT_FOREVER);

		if (*data4 != 0x188)
			RAW_ASSERT(0);
		
		raw_mq_receive (&app_queue2_mqueue,  &data5, &msg_length5, &pri5,  RAW_WAIT_FOREVER);

		if (*data5 != 0x188)
			RAW_ASSERT(0);
		
	
	}
}


void watch_task_mqueue(void * pParam) 
{
	while (1) {
		
		raw_sleep(RAW_TICKS_PER_SECOND / 2);
		vc_port_printf("watch_number_mqueue is %x\n", watch_number_mqueue);
	}
	

}

extern RAW_U8 test_started_raw;

void queue_test2_mqueue()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	raw_task_create(&test_queue_obj1_mqueue[1], (RAW_U8  *)"task1", 0,
	                         10, 0,  test_queue_stack11_mqueue, 
	                         TEST_TASK_STK_SIZE ,  driver_send_task_mqueue, 1); 


	raw_task_create(&test_queue_obj1_mqueue[2], (RAW_U8  *)"task2", 0,
	                         11, 10,   test_queue_stack22_mqueue, 
	                         TEST_TASK_STK_SIZE ,   net_receive_task_mqueue, 1); 



  
	raw_task_create(&test_queue_obj1_mqueue[3], (RAW_U8  *)"task3", 0,
	                         12, 10,   test_queue_stack33_mqueue, 
	                         TEST_TASK_STK_SIZE ,  app_receive_task1_mqueue, 1); 


	raw_task_create(&test_queue_obj1_mqueue[4], (RAW_U8  *)"task4", 0,
	                         13, 10,   test_queue_stack44_mqueue, 
	                         TEST_TASK_STK_SIZE ,  app_receive_task2_mqueue, 1); 	


	raw_task_create(&test_queue_obj1_mqueue[5], (RAW_U8  *)"task4", 0,
	                         14, 10,   test_queue_stack55_mqueue, 
	                         TEST_TASK_STK_SIZE ,  watch_task_mqueue, 1); 

	

}

