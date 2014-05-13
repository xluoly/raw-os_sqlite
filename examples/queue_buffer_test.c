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


/* 	2013-6  Created by jorya_txj
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



#define  TEST_TASK_STK_SIZE (10 * 1024)

static PORT_STACK test_queue_buffer_stack11[TEST_TASK_STK_SIZE];

static PORT_STACK test_queue_buffer_stack22[TEST_TASK_STK_SIZE];
static PORT_STACK test_queue_buffer_stack33[TEST_TASK_STK_SIZE];

static RAW_QUEUE_BUFFER q1;

static RAW_TASK_OBJ 		test_queue_buffer_obj[10];

static RAW_U32 queue1_buffer_space[1024];

extern void  vc_port_printf(char*   f,   ...);
void start_vc_timer(int tick_ms);



static RAW_U32 test_array[6] = {0x11223344, 0x55667788, 0x11223344, 0x99aabbcc, 0xee};
static RAW_U32 test_array2[6] = {0x8899aabb, 0xccddeeff};

//static RAW_U8 test_array[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
//static RAW_U8 test_array2[8] = {0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee};

static void queue_buffer_send_task(void * pParam) 
{	
	
	vc_port_printf("complex queue buffer test..........\n");
	
	raw_queue_buffer_create(&q1, "queue_buffer", queue1_buffer_space, 1024 * 4, 0xffff);
	
	
	while(1) {
		
		raw_queue_buffer_end_post(&q1, test_array2, 21);
		
		raw_sleep(100);

		raw_queue_buffer_end_post(&q1, test_array, 21);
		
		
	}

	
}



static void queue_buffer_receive_task(void * pParam) 
{	
	
	
	RAW_U32 msg[100];

	RAW_U32 msg_size;
	RAW_U32 count = 0;

	raw_memset(msg, 0, sizeof(msg));
	
	
	while(1) {
		
		raw_queue_buffer_receive(&q1, RAW_WAIT_FOREVER,msg, &msg_size);

		vc_port_printf("first data is %x\n", msg[0]);
		vc_port_printf("second data is %x\n", msg[1]);
		vc_port_printf("third data is %x\n", msg[2]);
		vc_port_printf("fouth data is %x\n", msg[3]);
		vc_port_printf("fifth data is %x\n", msg[4]);
		vc_port_printf("count is %d\n", count++);
		
	}

	
}


extern RAW_U8 test_started_raw;

void queue_buffer_test()
{

	if (test_started_raw) {

		return;

	}

	test_started_raw = 1;

	
	
	raw_task_create(&test_queue_buffer_obj[0], (RAW_U8  *)"task1", 0,
	                         2, 0,  test_queue_buffer_stack11, 
	                         TEST_TASK_STK_SIZE ,  queue_buffer_send_task, 1); 


	raw_task_create(&test_queue_buffer_obj[1], (RAW_U8  *)"task2", 0,
	                         11, 10,   test_queue_buffer_stack22, 
	                         TEST_TASK_STK_SIZE ,   queue_buffer_receive_task, 1); 



  

	

}




