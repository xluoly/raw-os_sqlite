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


/* 	2013-1  Created by jorya_txj
  *	xxxxxx   please added here
  */


#include <raw_api.h>
#include <mm/raw_page.h>
#include <mm/raw_malloc.h>
#include <raw_work_queue.h>

#include <fifo.h>
#include <lib_string.h>


extern RAW_U32 test_page_mem[1024*1024];
extern RAW_U8 test_started_raw;


#define  TEST_TASK_STK_SIZE 10240

static PORT_STACK test_work_queue_stack[TEST_TASK_STK_SIZE];


static RAW_TASK_OBJ test_work_obj;

static WORK_QUEUE_STRUCT wq;

static RAW_U32 count;
static void handler(RAW_U32 arg, void *msg)
{
	count++;
	vc_port_printf("count111 is %d\n", count);
	vc_port_printf("arg111 is %x\n", arg);
	vc_port_printf("msg111 is %p\n", msg);


}


static void handler2(RAW_U32 arg, void *msg)
{
	count++;
	vc_port_printf("count222 is %d\n", count);
	vc_port_printf("arg222 is %x\n", arg);
	vc_port_printf("msg222 is %p\n", msg);


}



static OBJECT_WORK_QUEUE_MSG work_msg[300];

static PORT_STACK work_stack1[10240];
static RAW_VOID *test_queue_work[100];

static RAW_U8 handler_toggle;

static void test_work_queue(void * pParam)
{
	RAW_U16 ret;
	
	global_work_queue_init(work_msg, 300);
	work_queue_create(&wq, 20, 10240, work_stack1, (RAW_VOID **)test_queue_work, 100);

	while (1) {
		if (handler_toggle == 0) {
			ret = sche_work_queue(&wq, 0x55667788, (void *)0x11223344, handler);
			RAW_ASSERT(ret == RAW_SUCCESS);
			handler_toggle = 1;
		}

		else {

			ret = sche_work_queue(&wq, 0x55667788, (void *)0x11223344, handler2);
			RAW_ASSERT(ret == RAW_SUCCESS);
			handler_toggle = 0;
		}
		
		raw_sleep(10);
	}


}


void work_queue_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;
	
	
	raw_task_create(&test_work_obj, (RAW_U8  *)"task2", 0,
	                         9, 0,  test_work_queue_stack, 
	                         TEST_TASK_STK_SIZE , test_work_queue, 1); 

}

